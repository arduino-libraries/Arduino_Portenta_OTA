#ifndef SSU_LZSS_H_
#define SSU_LZSS_H_

/**************************************************************************************
   INCLUDE
 **************************************************************************************/

#include <Arduino.h>
#include <stdint.h>
#include "Arduino_Portenta_OTA.h"

/**************************************************************************************
   FUNCTION DEFINITION
 **************************************************************************************/

void lzss_init(FILE * update_file_ptr, FILE * target_file_ptr, uint32_t const lzss_file_size, ArduinoPortentaOtaWatchdogResetFuncPointer wdog_feed_func_ptr);
void lzss_decode();
void lzss_flush();

/**************************************************************************************
   LZSS DECODER CLASS
 **************************************************************************************/


class LZSSDecoder {
public:

    /**
     * Build an LZSS decoder by providing a callback for storing the decoded bytes
     * @param putc_cbk: a callback that takes a char and stores it e.g. a callback to fwrite
     */
    LZSSDecoder(std::function<void(const uint8_t)> putc_cbk);

    /**
     * Build an LZSS decoder providing a callback for getting a char and putting a char
     * in this way you need to call decompress with no parameters
     * @param putc_cbk: a callback that takes a char and stores it e.g. a callback to fwrite
     * @param getc_cbk: a callback that returns the next char to consume
     *                  -1 means EOF, -2 means buffer is temporairly finished
     */
    LZSSDecoder(std::function<int()> getc_cbk, std::function<void(const uint8_t)> putc_cbk);

    /**
     * this enum describes the result of the computation of a single FSM computation
     * DONE: the decompression is completed
     * IN_PROGRESS: the decompression cycle completed successfully, ready to compute next
     * NOT_COMPLETED: the current cycle didn't complete because the available data is not enough
     */
    enum status: uint8_t {
        DONE,
        IN_PROGRESS,
        NOT_COMPLETED
    };

    /**
     * decode the provided buffer until buffer ends, then pause the process
     * @return DONE if the decompression is completed, NOT_COMPLETED if not
     */
    status decompress(uint8_t* const buffer=nullptr, uint32_t size=0);

    static const int LZSS_EOF = -1;
    static const int LZSS_BUFFER_EMPTY = -2;
private:
    // TODO provide a way for the user to set these parameters
    static const int EI = 11;             /* typically 10..13 */
    static const int EJ =  4;             /* typically 4..5 */
    static const int N = (1 << EI);       /* buffer size */
    static const int F = ((1 << EJ) + 1); /* lookahead buffer size */

    // algorithm specific buffer used to store text that could be later referenced and copied
    uint8_t buffer[N * 2];

    // this function gets 1 single char from the input buffer
    int getc();
    uint8_t* in_buffer = nullptr;
    uint32_t available = 0;

    status handle_state();

    // get 1 bit from the available input buffer
    int getbit(uint8_t n);
    // the following 2 are variables used by getbits
    uint32_t buf, buf_size=0;

    enum FSM_STATES: uint8_t {
        FSM_0       = 0,
        FSM_1       = 1,
        FSM_2       = 2,
        FSM_3       = 3,
        FSM_EOF
    } state;

    // these variable are used in a decode session and specific to the old C implementation
    // there is no documentation about their meaning
    int i, r;

    std::function<void(const uint8_t)> put_char_cbk;
    std::function<uint8_t()> get_char_cbk;

    inline void putc(const uint8_t c) { if(put_char_cbk) { put_char_cbk(c); } }

    // get the number of bits the FSM will require given its state
    uint8_t bits_required(FSM_STATES s);
};

#endif /* SSU_LZSS_H_ */
