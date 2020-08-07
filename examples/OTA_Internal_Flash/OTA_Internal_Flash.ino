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

  Serial.println("*****OTA from Internal Flash*****");
  Serial.println("Select the storage mode. Press:");
  Serial.println("0: raw device, no offset");
  Serial.println("1: have a filesystem, use offset as partition start");

  while (!Serial.available());
  int mode = Serial.readStringUntil('\n').toInt();

  int started = millis();

  if (mode == 0) {
    Serial.println("OTA from raw flash");
    OTAPortenta.begin(InternalFlashRaw);
  } else if (mode == 1) {
    Serial.println("OTA from flash with filesystem");
    //OTAPortenta.begin(InternalFlashOffset);
  } else {
    Serial.println("Invalid mode");
    while(1) {}
  }

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, LOW);
  delay(5000);
  digitalWrite(LEDB, HIGH);

  Serial.println("Restarting the system with updated fw...");

}

void loop() {
  OTAPortenta.update();
}
