/*
 This example demonstrates how to use to update
 the firmware of the Arduino Portenta H7 using
 a firmware image stored in the internal flash of
 the MCU.

 Before uploading this sketch:
   Flash OTA_Usage_Portenta.ino.bin through dfu-util at address 0x08080000
 */

#include "ArduinoOTAPortenta.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("*****OTA from Internal Flash*****");
  ArduinoOTAPortenta_InternalFlash ota(INTERNAL_FLASH_OFFSET, 0x80000);

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, LOW);
  delay(5000);
  digitalWrite(LEDB, HIGH);

  ota.update();
}

void loop()
{

}
