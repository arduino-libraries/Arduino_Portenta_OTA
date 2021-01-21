/*
 This example demonstrates how to use to update
 the firmware of the Arduino Portenta H7 using
 a firmware image stored in the internal flash of
 the MCU.

 Before uploading this sketch:
   Flash OTA_Usage_Portenta.ino.bin through dfu-util at address 0x08080000
 */

#include "Arduino_Portenta_OTA.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("*****OTA from Internal Flash*****");

  Arduino_Portenta_OTA_InternalFlash ota(INTERNAL_FLASH_OFFSET, 0x80000);
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
