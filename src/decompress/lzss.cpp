/**************************************************************************************
   INCLUDE
 **************************************************************************************/

#include "lzss.h"

#include <stdlib.h>

/**************************************************************************************
   GLOBAL VARIABLES
 **************************************************************************************/

static FILE * update_file = nullptr;
static FILE * target_file = nullptr;

static ArduinoPortentaOtaWatchdogResetFuncPointer wdog_feed_func = nullptr;
static LZSSDecoder* decoder = nullptr;

/**************************************************************************************
   PUBLIC FUNCTIONS
 **************************************************************************************/

void lzss_init(
        FILE * update_file_ptr,
        FILE * target_file_ptr,
        uint32_t const lzss_file_size,
        ArduinoPortentaOtaWatchdogResetFuncPointer wdog_feed_func_ptr) {
    update_file = update_file_ptr;
    target_file = target_file_ptr;
    wdog_feed_func = wdog_feed_func_ptr;

    if(decoder != nullptr) {
        delete decoder;
        decoder = nullptr;
    }

    decoder = new LZSSDecoder(
        [target_file](const uint8_t c) {
            fwrite(&c, 1, 1, target_file);
        }
    );
}

void lzss_flush() {
    fflush(target_file);
}

void lzss_decode() {
    if(decoder == nullptr) {
        return;
    }
    const size_t buf_size = 64;
    uint8_t buffer[buf_size];
    size_t res = 0;

    do {
        if(wdog_feed_func) {
            wdog_feed_func();
        }
        res = fread(buffer, sizeof(uint8_t), buf_size, update_file);
        decoder->decompress(buffer, res);
    } while(res == buf_size);
}


/**************************************************************************************
   LZSS DECODER CLASS IMPLEMENTATION
 **************************************************************************************/

// get the number of bits the algorithm will try to get given the state
uint8_t LZSSDecoder::bits_required(LZSSDecoder::FSM_STATES s) {
    switch(s) {
    case FSM_0:
        return 1;
    case FSM_1:
        return 8;
    case FSM_2:
        return EI;
    case FSM_3:
        return EJ;
    default:
        return 0;
    }
}

LZSSDecoder::LZSSDecoder(std::function<int()> getc_cbk, std::function<void(const uint8_t)> putc_cbk)
: available(0), state(FSM_0), put_char_cbk(putc_cbk), get_char_cbk(getc_cbk) {
    for (int i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
}


LZSSDecoder::LZSSDecoder(std::function<void(const uint8_t)> putc_cbk)
: available(0), state(FSM_0), put_char_cbk(putc_cbk), get_char_cbk(nullptr) {
    for (int i = 0; i < N - F; i++) buffer[i] = ' ';
    r = N - F;
}

LZSSDecoder::status LZSSDecoder::handle_state() {
    LZSSDecoder::status res = IN_PROGRESS;

    int c = getbit(bits_required(this->state));

    if(c == LZSS_BUFFER_EMPTY) {
        res = NOT_COMPLETED;
    } else if (c == LZSS_EOF) {
        res = DONE;
        this->state = FSM_EOF;
    } else {
        switch(this->state) {
            case FSM_0:
                if(c) {
                    this->state = FSM_1;
                } else {
                    this->state = FSM_2;
                }
                break;
            case FSM_1:
                putc(c);
                buffer[r++] = c;
                r &= (N - 1); // equivalent to r = r % N when N is a power of 2

                this->state = FSM_0;
                break;
            case FSM_2:
                this->i = c;
                this->state = FSM_3;
                break;
            case FSM_3: {
                int j = c;

                // This is where the actual decompression takes place: we look into the local buffer for reuse
                // of byte chunks. This can be improved by means of memcpy and by changing the putc function
                // into a put_buf function in order to avoid buffering on the other end.
                // TODO improve this section of code
                for (int k = 0; k <= j + 1; k++) {
                    c = buffer[(this->i + k) & (N - 1)]; // equivalent to buffer[(i+k) % N] when N is a power of 2
                    putc(c);
                    buffer[r++] = c;
                    r &= (N - 1); // equivalent to r = r % N
                }
                this->state = FSM_0;

                break;
            }
            case FSM_EOF:
                break;
        }
    }

    return res;
}

LZSSDecoder::status LZSSDecoder::decompress(uint8_t* const buffer, uint32_t size) {
    if(!get_char_cbk) {
        this->in_buffer = buffer;
        this->available += size;
    }

    status res = IN_PROGRESS;

    while((res = handle_state()) == IN_PROGRESS);

    this->in_buffer = nullptr;

    return res;
}

int LZSSDecoder::getbit(uint8_t n) { // get n bits from buffer
    int x=0, c;

    // if the local bit buffer doesn't have enough bit get them
    while(buf_size < n) {
        switch(c=getc()) {
        case LZSS_EOF:
        case LZSS_BUFFER_EMPTY:
            return c;
        }
        buf <<= 8;

        buf |= (uint8_t)c;
        buf_size += sizeof(uint8_t)*8;
    }

    // the result is the content of the buffer starting from msb to n successive bits
    x = buf >> (buf_size-n);

    // remove from the buffer the read bits with a mask
    buf &= (1<<(buf_size-n))-1;

    buf_size-=n;

    return x;
}

int LZSSDecoder::getc() {
    int c;

    if(get_char_cbk) {
        c = get_char_cbk();
    } else if(in_buffer == nullptr || available == 0) {
        c = LZSS_BUFFER_EMPTY;
    } else {
        c = *in_buffer;
        in_buffer++;
        available--;
    }
    return c;
}
