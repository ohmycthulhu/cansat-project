#include "include/test.cpp"
#include <TimerOne.h>

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int i = 0; i < 5; i++) {
    test::test();
  }
}

void loop() {
  delay(2000);
}
