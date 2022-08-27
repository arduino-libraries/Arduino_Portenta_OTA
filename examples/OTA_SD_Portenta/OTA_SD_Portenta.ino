/*
 * This example demonstrates how to use to update the firmware of the Arduino Portenta H7 using
 * a firmware image stored on the SD.
 *
 * Steps:
 *   1) Create a sketch for the Portenta H7 and verify
 *      that it both compiles and works on a board.
 *   2) In the IDE select: Sketch -> Export compiled Binary.
 *   3) Create an OTA update file utilising the tools 'lzss.py' and 'bin2ota.py' stored in
 *      https://github.com/arduino-libraries/ArduinoIoTCloud/tree/master/extras/tools .
 *      A) ./lzss.py --encode SKETCH.bin PORTENTA_H7_M7.lzss
 *      B) ./bin2ota.py PORTENTA_H7_M7.lzss PORTENTA_H7_M7.ota
 *   4) Upload the OTA file to a network reachable location, e.g. OTA_Usage_Portenta.ino.PORTENTA_H7_M7.ota
 *      has been uploaded to: http://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.PORTENTA_H7_M7.ota
 *   5) Perform an OTA update via steps outlined below.
 */

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <Arduino_Portenta_OTA.h>

#include <WiFi.h>

#include "arduino_secrets.h" 

/******************************************************************************
 * CONSTANT
 ******************************************************************************/

/* Please enter your sensitive data in the Secret tab/arduino_secrets.h */
static char const SSID[] = SECRET_SSID;  /* your network SSID (name) */
static char const PASS[] = SECRET_PASS;  /* your network password (use for WPA, or use as key for WEP) */

static char const OTA_FILE_LOCATION[] = "http://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.PORTENTA_H7_M7.ota";

/******************************************************************************
 * SETUP/LOOP
 ******************************************************************************/

void setup()
{
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
    delay(10000);
  }
  Serial.print  ("You're connected to '");
  Serial.print  (WiFi.SSID());
  Serial.println("'");

  //Arduino_Portenta_OTA_SD ota(SD_FATFS, 0);
  Arduino_Portenta_OTA_SD ota(SD_FATFS_MBR, 1);
  Arduino_Portenta_OTA::Error ota_err = Arduino_Portenta_OTA::Error::None;

  if (!ota.isOtaCapable())
  {
    Serial.println("Higher version bootloader required to perform OTA.");
    Serial.println("Please update the bootloader.");
    Serial.println("File -> Examples -> Portenta_System -> PortentaH7_updateBootloader");
    return;
  }

  Serial.println("Initializing OTA storage");
  if ((ota_err = ota.begin()) != Arduino_Portenta_OTA::Error::None)
  {
    Serial.print  ("Arduino_Portenta_OTA::begin() failed with error code ");
    Serial.println((int)ota_err);
    return;
  }


  Serial.println("Starting download to SD ...");
  int const ota_download = ota.download(OTA_FILE_LOCATION, false /* is_https */);
  if (ota_download <= 0)
  {
    Serial.print  ("Arduino_Portenta_OTA_SD::download failed with error code ");
    Serial.println(ota_download);
    return;
  }
  Serial.print  (ota_download);
  Serial.println(" bytes stored.");


  Serial.println("Decompressing LZSS compressed file ...");
  int const ota_decompress = ota.decompress();
  if (ota_decompress < 0)
  {
    Serial.print("Arduino_Portenta_OTA_SD::decompress() failed with error code");
    Serial.println(ota_decompress);
    return;
  }
  Serial.print(ota_decompress);
  Serial.println(" bytes decompressed.");


  Serial.println("Storing parameters for firmware update in bootloader accessible non-volatile memory");
  if ((ota_err = ota.update()) != Arduino_Portenta_OTA::Error::None)
  {
    Serial.print  ("Arduino_Portenta_OTA::update() failed with error code ");
    Serial.println((int)ota_err);
    return;
  }

  Serial.println("Performing a reset after which the bootloader will update the firmware.");
  Serial.println("Hint: Portenta H7 LED will blink Red-Blue-Green.");
  delay(1000);
  ota.reset();
}

void loop()
{

}