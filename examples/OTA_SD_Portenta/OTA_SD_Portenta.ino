/*
 This example demonstrates how to use the OTA on Portenta, 
 flashing the binary stored in the SD card.
 Before uploading this sketch:
 Upload OTA_Usage_Portenta.ino and follow the instructions
*/
#include "ArduinoOTAPortenta.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("*****OTA from SD*****");

  int started = millis();

  Serial.print("Opening SD card...");

  OTAPortenta.setUpdateLen(131728);
  OTAPortenta.begin(SD_OFFSET,10240);

  Serial.println("OK!");

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, LOW);
  delay(5000);
  digitalWrite(LEDB, HIGH);

  Serial.println("Restarting the system with updated fw...");

}

void loop() {
  OTAPortenta.update();
}
