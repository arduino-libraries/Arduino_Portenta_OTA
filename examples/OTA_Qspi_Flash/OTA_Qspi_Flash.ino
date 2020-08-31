/*
 This example demonstrates how to use the OTA on Portenta, 
 flashing the binary stored in the Internal Flash in Bank1.
 With the library ArduinoOTAPortenta it is currently possible to test
 only the storage type INTERNAL_FLASH_OFFSET. 
 Before uploading this sketch:
 Flash OTA_Usage_Portenta.ino.bin through dfu-util at address 0x08080000
*/
#include "ArduinoOTAPortenta.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("*****OTA from M4 Flash*****");
  /*
  Serial.println("Select the storage mode. Press:");
  Serial.println("0: raw device, no offset");
  Serial.println("1: have a filesystem, use offset as partition start");

  while (!Serial.available());
  int mode = Serial.readStringUntil('\n').toInt();
  */

  int started = millis();
  /*
  if (mode == 0) {
    Serial.println("OTA from raw flash");
    OTAPortenta.begin(INTERNAL_FLASH_OFFSET,0x80000);
  } else if (mode == 1) {
    Serial.println("OTA from flash with filesystem");
    //OTAPortenta.begin(InternalFlashOffset);
  } else {
    Serial.println("Invalid mode");
    while(1) {}
  }
  */
  OTAPortenta.setUpdateLen(131728);
  OTAPortenta.begin(QSPI_FLASH_FATFS_MBR,2048); //2048

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, LOW);
  delay(5000);
  digitalWrite(LEDB, HIGH);

  Serial.println("Restarting the system with updated fw...");

}

void loop() {
  OTAPortenta.update();
}
