/*
 This example demonstrates how to use to update
 the firmware of the Arduino Portenta H7 using
 a firmware image stored on a SD card.
 */

#include "Arduino_OTA_Portenta.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("*****OTA from SD*****");
  Arduino_OTA_Portenta_SD ota(SD_OFFSET, 10240);
  ota.setUpdateLen(131728);

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, LOW);
  delay(5000);
  digitalWrite(LEDB, HIGH);

  ota.update();
}

void loop()
{

}
