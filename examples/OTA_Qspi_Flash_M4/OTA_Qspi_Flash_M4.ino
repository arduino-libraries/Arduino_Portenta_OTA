/*
 * This example demonstrates how to update the firmware of the Arduino Portenta H7 M4 core
 * using a firmware image stored on the QSPI.
 *
 * WARNING: Only 1MB M7 + 1MB M4 flash split is supported
 *
 * Steps:
 *   1) Create a sketch for the Portenta H7 M4 core using 1MB M7 + 1MB M4 flash split and verify
 *      that it both compiles and works on a board.
 *   2) In the IDE select: Sketch -> Export compiled Binary.
 *   3) Create an OTA update file utilising the tools 'lzss.py' and 'bin2ota.py' stored in
 *      https://github.com/arduino-libraries/ArduinoIoTCloud/tree/master/extras/tools .
 *      A) ./lzss.py --encode SKETCH.bin SKETCH.lzss
 *      B) ./bin2ota.py PORTENTA_H7_M7 SKETCH.lzss SKETCH.ota
 *   4) Upload the OTA file to a network reachable location, e.g. OTA_Usage_Portenta.ino.PORTENTA_H7_M4.ota
 *      has been uploaded to: http://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.PORTENTA_H7_M4.ota
 *   5) Perform an OTA update via steps outlined below.
 */

/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include <Arduino_Portenta_OTA.h>

#include <WiFi.h>

#include "RPC.h"

#include "arduino_secrets.h" 

/******************************************************************************
 * CONSTANT
 ******************************************************************************/

/* Please enter your sensitive data in the Secret tab/arduino_secrets.h */
static char const SSID[] = SECRET_SSID;  /* your network SSID (name) */
static char const PASS[] = SECRET_PASS;  /* your network password (use for WPA, or use as key for WEP) */

#if defined(ARDUINO_PORTENTA_H7_M7)
static char const OTA_FILE_LOCATION[] = "https://downloads.arduino.cc/ota/OTA_Usage_Portenta.ino.PORTENTA_H7_M4.ota";
#else
#error "Board not supported"
#endif

/******************************************************************************
 * SETUP/LOOP
 ******************************************************************************/

void setup()
{
  // Initialize RPC library; this also boots the M4 core
  RPC.begin();
  
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("Do you want to install/update the M4 core firmware? Y/[n]");
  if(!waitResponse()) {
    Serial.println("Nothing to do. It's now safe to reboot or disconnect your board.");
    return;
  }

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

  Arduino_Portenta_OTA_QSPI ota(QSPI_FLASH_FATFS_MBR, 2);
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


  Serial.println("Starting download to QSPI ...");
  int const ota_download = ota.download(OTA_FILE_LOCATION, true /* is_https */);
  if (ota_download <= 0)
  {
    Serial.print  ("Arduino_Portenta_OTA_QSPI::download failed with error code ");
    Serial.println(ota_download);
    return;
  }
  Serial.print  (ota_download);
  Serial.println(" bytes stored.");


  Serial.println("Decompressing LZSS compressed file ...");
  int const ota_decompress = ota.decompress();
  if (ota_decompress < 0)
  {
    Serial.print("Arduino_Portenta_OTA_QSPI::decompress() failed with error code");
    Serial.println(ota_decompress);
    return;
  }
  Serial.print(ota_decompress);
  Serial.println(" bytes decompressed.");


  Serial.println("Storing parameters for firmware update in bootloader accessible non-volatile memory ...");
  if ((ota_err = ota.update()) != Arduino_Portenta_OTA::Error::None)
  {
    Serial.print  ("ota.update() failed with error code ");
    Serial.println((int)ota_err);
    return;
  }

  Serial.println("Performing a reset after which the bootloader will update the firmware.");
  Serial.println("Hint: Portenta H7 LED will blink Red-Blue-Green.");
  delay(1000); /* Make sure the serial message gets out before the reset. */
  ota.reset();
}

void loop()
{

}

bool waitResponse() {
  bool confirmation = false;
  while (confirmation == false) {
    if (Serial.available()) {
      char choice = Serial.read();
      switch (choice) {
        case 'y':
        case 'Y':
          confirmation = true;
          return true;
          break;
        case 'n':
        case 'N':
          confirmation = true;
          return false;
          break;
        default:
          continue;
      }
    }
  }
}
