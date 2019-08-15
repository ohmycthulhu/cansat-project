//#include "include/test.cpp"
#include <SoftwareSerial.h>
const int xbeeRX = 6, xbeeTX = 7;
const int gpsTX = 5;

SoftwareSerial ss(xbeeRX, xbeeTX);
SoftwareSerial gps(0, gpsTX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ss.begin(9600);
  gps.begin(9600);
  gps.listen();
  // for (int i = 0; i < 5; i++) {
  //  test::test();
  //}
  // ss.listen();
}

void loop() {
  bool isRead = false;
  Serial.println("Is listening gps? " + String(gps.isListening()));
  
  while (ss.available()) {
    Serial.print((char)ss.read());
    isRead = true;
  }
  if (isRead) {
    Serial.println();
  }
  Serial.println("Is listening gps? " + String(gps.isListening()));
  ss.listen();
  delay(2000);
  isRead = false;
  Serial.println("Is listening gps? " + String(gps.isListening()));
  
  while (ss.available()) {
    Serial.print((char)ss.read());
    isRead = true;
  }
  if (isRead) {
    Serial.println();
  }
  delay(2000);
  gps.listen();
  }
