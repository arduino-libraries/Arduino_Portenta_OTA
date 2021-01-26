/*
 This example demonstrates how to use to update
 the firmware of the Arduino Portenta H7 using
 a firmware image stored on a SD card.
 */

#include "Arduino_Portenta_OTA.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("*****OTA from SD*****");
  Arduino_Portenta_OTA_SD ota(SD_OFFSET, 10240);
  Arduino_Portenta_OTA::Error ota_err = Arduino_Portenta_OTA::Error::None;

  Serial.println("Initializing OTA storage");
  if ((ota_err = ota.begin()) != Arduino_Portenta_OTA::Error::None)
  {
    Serial.print  ("ota.begin() failed with error code ");
    Serial.println((int)ota_err);
    return;
  }

  /* This function sets the precise length of update binary, in this case of OTA_Usage_Portenta.ino.PORTENTA_H7_M7.bin */
  ota.setUpdateLen(131728);

  Serial.println("Storing parameters for firmware update in bootloader accessible non-volatile memory");
  if ((ota_err = ota.update()) != Arduino_Portenta_OTA::Error::None)
  {
    Serial.print  ("ota.update() failed with error code ");
    Serial.println((int)ota_err);
    return;
  }

  Serial.println("Performing a reset after which the bootloader will update the firmware.");
  ota.reset();
}

void loop()
{

}
