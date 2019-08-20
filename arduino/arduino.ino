#include <MemoryFree.h>
#include "include/sensors.cpp"
#include "include/xbeeinterface.cpp"

void makeOneLifecycle () {
  using namespace commands;
  using namespace xbee;
  Commands executedCommand;
  // Check xbee for command
  if (XBeeInterface::isThereCommand()) {
    auto command = xbee::XBeeInterface::getCommand();
    auto status = commands::execute(command, &executedCommand);
    XBeeInterface::send(String((int)executedCommand) + "," + String((int)status), MessageType::COMMAND_REPORT);
    return;
  }
  // Read data from sensors
  Packet p = sensors::getPacket();
  // Pass packet to commands
  auto commandStatus = commands::execute(p, &executedCommand);
  if (commandStatus != Statuses::NO_COMMAND) {
    XBeeInterface::send(String((int)executedCommand) + "," + String((int)commandStatus), xbee::MessageType::COMMAND_REPORT);
    return;
  }
  // Send packet
  Serial.println(p.toString());
  auto parser = sensors::getGPSParser();
  XBeeInterface::send(p.toString(), xbee::MessageType::TELEMETRY);
}

void setup() {
  // For Debug
  Serial.begin(9600);
  
  sensors::initialize();
  xbee::XBeeInterface::initialize();

  sensors::listenGPS();
  
}

void smartDelay (long ms) {
  static unsigned long lastSwitchTime = 0;
  static unsigned short state = 1;
  auto start = millis();
  do {
    switch(state) {
      case 0:
        sensors::listen();
        if (millis() - lastSwitchTime > sensors::listenTimeout) {
          xbee::XBeeInterface::listenXBee();
          lastSwitchTime = millis();
          state = 1;
        }
        break;
      case 1:
        xbee::XBeeInterface::listen();
        if (millis() - lastSwitchTime > xbee::XBeeInterface::listenTimeout) {
          sensors::listenGPS();
          lastSwitchTime = millis();
          state = 0;
        }
        break;
    }
  } while(millis() - start < ms);
}

void loop() {  
  
  makeOneLifecycle();
  smartDelay(700);
}
