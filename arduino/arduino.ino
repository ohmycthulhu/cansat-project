#include "include/test.cpp"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
}

void loop() {
  test::test();
  delay(10000);
}
