/*
 * This example demonstrates how to download a lzss file and decompress it in two ways:
 * -1 download the file on the filesystem and then decompress the downloaded file on the filesystem
 * -2 download and decompress the file on the fly
 * this sketch also provides a comparison in terms of speed and execution time
 *
 */

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <Arduino_Portenta_OTA.h>

#include <WiFi.h>

#include "arduino_secrets.h"
#include <decompress/lzss.h>

/******************************************************************************
 * CONSTANT
 ******************************************************************************/

/* Please enter your sensitive data in the Secret tab/arduino_secrets.h */
static char const SSID[] = SECRET_SSID;  /* your network SSID (name) */
static char const PASS[] = SECRET_PASS;  /* your network password (use for WPA, or use as key for WEP) */


#if defined(ARDUINO_NICLA_VISION)
static char const URL_FILE[] = "https://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.NICLA_VISION.ota";
#elif defined(ARDUINO_PORTENTA_H7_M7)
static char const URL_FILE[] = "https://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.PORTENTA_H7_M7.ota";
#elif defined(ARDUINO_OPTA)
static char const URL_FILE[] = "https://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.OPTA.ota";
#elif defined(ARDUINO_GIGA)
static char const URL_FILE[] = "https://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.GIGA.ota";
#else
#error "Board not supported"
#endif

static char const DOWNLOAD_DESTINATION[] =      "/fs/UPDATE.BIN.LZSS";
static char const DECOMPRESSED_DESTINATION[] =  "/fs/UPDATE.BIN";

LZSSDecoder *decoder = nullptr;
FILE* download_target = nullptr;

/******************************************************************************
 * SETUP/LOOP
 ******************************************************************************/
void decompress_on_the_fly_cbk(const char*, uint32_t);
void putc_file(const uint8_t c);

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("Communication with WiFi module failed!");
        return;
    }

    int status = WL_IDLE_STATUS;
    while (status != WL_CONNECTED)
    {
        Serial.print  ("Attempting to connect to '");
        Serial.print  (SSID);
        Serial.println("'");
        status = WiFi.begin(SSID, PASS);
        if(status != WL_CONNECTED) {
            delay(10000);
        }
    }
    Serial.print  ("You're connected to '");
    Serial.print  (WiFi.SSID());
    Serial.println("'");

    // Init fs
    mbed::BlockDevice * _bd_raw_qspi = mbed::BlockDevice::get_default_instance();;
    auto _bd_qspi = new mbed::MBRBlockDevice(_bd_raw_qspi, 2);
    auto _fs_qspi = new mbed::FATFileSystem("fs");
    int const err_mount = _fs_qspi->mount(_bd_qspi);
    if (err_mount) {
        Serial.print("Error while mounting the filesystem. Err = ");
        Serial.println(err_mount);
        return;
    }

    MbedSocketClass * socket = static_cast<MbedSocketClass*>(&WiFi);
    remove(DOWNLOAD_DESTINATION);
    remove(DECOMPRESSED_DESTINATION);

    uint32_t start;
    int bytes;
    float elapsed, speed;
    start = millis();
    Serial.println("Starting download to QSPI ...");
    bytes = socket->download(URL_FILE, DOWNLOAD_DESTINATION, true /* is_https */);
    if (bytes <= 0)
    {
        Serial.print  ("MbedSocketClass::download failed with error code ");
        Serial.println(bytes);
        return;
    }
    Serial.print  (bytes);
    Serial.println(" bytes stored.");

    elapsed = (millis()-start)/1000.0; // elapsed expressed in seconds
    speed = (bytes/elapsed)/1024;

    Serial.print("download elapsed ");
    Serial.print(elapsed);
    Serial.print("s speed: ");
    Serial.print(speed);
    Serial.println("KBps");

    FILE* downloaded_file = fopen(DOWNLOAD_DESTINATION, "rb");
    FILE* decompressed_file = fopen(DECOMPRESSED_DESTINATION, "wb");

    start = millis();
    lzss_init(downloaded_file, decompressed_file, bytes, nullptr);

    lzss_decode();
    /* Write the data remaining in the write buffer to
    * the file.
    */
    lzss_flush();

    elapsed = (millis()-start)/1000.0; // elapsed expressed in seconds

    Serial.print("decompress elapsed ");
    Serial.print(elapsed);
    Serial.print("s");
    Serial.print(" size ");
    Serial.println(ftell(decompressed_file));

    fclose(downloaded_file);
    fclose(decompressed_file);

    // On the fly decompression
    remove(DOWNLOAD_DESTINATION);
    remove(DECOMPRESSED_DESTINATION);

    download_target = fopen(DECOMPRESSED_DESTINATION, "wb");
    decoder = new LZSSDecoder(putc_file);

    Serial.println("Starting download & decompress on the fly");
    start = millis();
    bytes = socket->download(URL_FILE, true /* is_https */, decompress_on_the_fly_cbk);
    if (bytes <= 0)
    {
        Serial.print  ("MbedSocketClass::download failed with error code ");
        Serial.println(bytes);
        return;
    }

    Serial.print("downloaded ");
    Serial.print(bytes);
    Serial.print(" bytes ");

    elapsed = (millis()-start)/1000.0; // elapsed expressed in seconds
    speed = (bytes/elapsed)/1024;

    Serial.print  (ftell(download_target));
    Serial.println(" bytes stored.");

    Serial.print("download elapsed ");
    Serial.print(elapsed);
    Serial.print("s speed: ");
    Serial.print(speed);
    Serial.println("KBps");

    delete decoder;
    fclose(download_target);
}

void loop() {
}

void decompress_on_the_fly_cbk(const char* buffer, uint32_t size) {
    decoder->decompress((uint8_t*)buffer, size);
}

void putc_file(const uint8_t c) {
    fwrite(&c, 1, 1, download_target);
}

