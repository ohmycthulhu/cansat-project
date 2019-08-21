#include <MemoryFree.h>
#include "include/sensors.cpp"
#include "include/xbeeinterface.cpp"

void makeOneLifecycle () {
  using namespace commands;
  using namespace xbee;
  Commands executedCommand;
  // Check xbee for command
  if (xbee::isThereCommand()) {
    auto command = xbee::getCommand();
    auto status = commands::execute(command, &executedCommand);
    xbee::send(String((int)executedCommand) + "," + String((int)status), MessageType::COMMAND_REPORT);
    return;
  }
  // Read data from sensors
  Packet p = sensors::getPacket();
  // Pass packet to commands
  auto commandStatus = commands::execute(p, &executedCommand);
  if (commandStatus != Statuses::NO_COMMAND) {
    xbee::send(String((int)executedCommand) + "," + String((int)commandStatus), xbee::MessageType::COMMAND_REPORT);
    return;
  }
  // Send packet
  Serial.println(p.toString());
  auto parser = sensors::getGPSParser();
  xbee::send(p.toString(), xbee::MessageType::TELEMETRY);
}

void setup() {
  // For Debug
  Serial.begin(9600);
  
  sensors::initialize();
  xbee::initialize();

  sensors::listenGPS();
  
}

void smartDelay (long ms) {
  static unsigned long lastSwitchTime = 0;
  static unsigned short state = 1;
  auto start = millis();
  do {
    switch(state) {
      case 0:
        if (millis() - lastSwitchTime > sensors::listenTimeout) {
          lastSwitchTime = millis();
          state = 1;
          continue;
        }
        sensors::listen();
        break;
      case 1:
        if (millis() - lastSwitchTime > xbee::listenTimeout) {
          lastSwitchTime = millis();
          state = 0;
          continue;
        }
        xbee::listen();
        break;
    }
  } while(millis() - start < ms);
}

void loop() {  
  
  makeOneLifecycle();
  smartDelay(700);
}
