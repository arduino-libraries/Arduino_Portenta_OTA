/*
 Usage
 This example demonstrates how to use the Arduino_OTA_Portenta library to update a 
 sketch on any Portenta via the storage types allowed by the board.
 Steps to update sketch:
 1) Upload this sketch or any other sketch (this one lights the LED up with different colours).
 2) In the IDE select: Sketch -> Export compiled Binary
 3) Open the location of the sketch and to choose the next step according to the desired storage type:
    - SD: copy the binary to the SD with the name "UPDATE.BIN"
    - INTERNAL FLASH:
    - QSPI Flash:
 5) Upload the sketch OTA_*_Portenta.ino
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
{ //led BLUE ON
  setLed(1, 0, 0);
  delay(1000);
  //led GREEN ON
  setLed(0, 1, 0);
  delay(1000);
  //led RED ON
  setLed(0, 0, 1);
  delay(1000);
}
