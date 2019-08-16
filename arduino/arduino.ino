#include <TimerOne.h>
#include <MemoryFree.h>
#include "include/sensors.cpp"
#include "include/xbeeinterface.cpp"


void listenSerials() {
  using namespace sensors;
  using namespace xbee;
  long currentTime = millis();
  if (Sensors::shouldInterrupt(millis)) {
    XBeeInterface::listenXBee();
  } else {
    if (XBeeInterface::shouldInterrupt(millis)) {
      Sensors::listenGPS();
    }
  }
  sensors::Sensors::listen();
  xbee::XBeeInterface::listen();
}

void makeOneLifecycle () {
  // Read data from sensors
  Packet p = sensors::Sensors::getPacket();
  // Pass packet to commands
   auto commandStatus = commands::CommandsInterface::execute(p);
  if (commandStatus != commands::Statuses::NO_COMMAND) {
    xbee::XBeeInterface::send(String((int)commandStatus), xbee::MessageType::COMMAND_REPORT);
  }
  // Send packet
  xbee::XBeeInterface::send(p.toString(), xbee::MessageType::TELEMETRY);
}
void setup() {
  Serial.begin(9600);

  sensors::Sensors::initialize();
  xbee::XBeeInterface::initialize();

  Timer1.initialize(1e4); // 10 ms = 10^4
  Timer1.attachInterrupt(listenSerials);
}

void loop() {
  makeOneLifecycle();
  delay(500);
}
