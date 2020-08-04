#define OTA_STORAGE_PORTENTA_SD

//#include <ArduinoIoTCloud.h>

#include "ArduinoOTAPortenta.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  Serial.println("Hello");

  int started = millis();
  
  OTAPortenta.begin(SD);

  pinMode(LEDB, OUTPUT);
  digitalWrite(LEDB, LOW);
  delay(5000);
  digitalWrite(LEDB, HIGH);

  //setDebugMessageLevel(2);
  //OTAPortenta.printDebugInfo();
}

void loop() {
  // put your main code here, to run repeatedly:
  OTAPortenta.update();
}
