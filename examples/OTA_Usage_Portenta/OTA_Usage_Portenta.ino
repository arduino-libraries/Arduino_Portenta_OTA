/*
 This sketch can be used to generate an example binary that can be uploaded to Portenta via OTA.
 It needs to be used together with
  - 'OTA_Qspi_Flash.ino' if you want to use the QSPI Flash as storage system
  OR
  - 'OTA_SD_Portenta.ino' if you want to use the SD card as storage system

 Steps to test OTA on Portenta:
 1) Upload this sketch or any other sketch (this one lights up the RGB LED with different colours).
 2) In the IDE select: Sketch -> Export compiled Binary
 3) Upload the exported binary to a server
 4) Choose a storage mechanism (SD or QSPI), open the related OTA_*_Portenta.ino sketch,
    eventually update the OTA_FILE_LOCATION
 5) Upload the sketch OTA_*_Portenta.ino to perform OTA via SD or QSPI Flash
*/

void setLed(int blue, int gree, int red) {
  if (blue == 1) {
    digitalWrite(LEDB, LOW);
  }
  else {
    digitalWrite(LEDB, HIGH);
  }

  if (gree == 1) {
    digitalWrite(LEDG, LOW);
  }
  else {
    digitalWrite(LEDG, HIGH);
  }

  if (red == 1) {
    digitalWrite(LEDR, LOW);
  }
  else {
    digitalWrite(LEDR, HIGH);
  }
}


void setup()
{
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);
}

void loop()
{ // Blue LED on
  setLed(1, 0, 0);
  delay(1000);
  // Green LED on
  setLed(0, 1, 0);
  delay(1000);
  // Red LED on
  setLed(0, 0, 1);
  delay(1000);
}
