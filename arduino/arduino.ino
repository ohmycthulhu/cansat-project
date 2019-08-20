#include <TimerOne.h>
#include <MemoryFree.h>
#include "include/sensors.cpp"
#include "include/xbeeinterface.cpp"


void listenSerials() {
  using namespace xbee;
  long currentTime = millis();
  if (sensors::shouldInterrupt(millis())) {
     XBeeInterface::listenXBee();
  } else {
    if (XBeeInterface::shouldInterrupt(millis())) {
      sensors::listenGPS();
    }
  }
  sensors::listen();
  xbee::XBeeInterface::listen();
}

void makeOneLifecycle () {
  using namespace commands;
  using namespace xbee;
  Commands executedCommand;
  // Check xbee for command
  if (XBeeInterface::isThereCommand()) {
    auto command = xbee::XBeeInterface::getCommand();
    auto status = commands::execute(command, &executedCommand);
    Serial.println("Got command - " + command);
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
  Serial.println("Successful checksums: " + String(parser.passedChecksum()) + ". Failed checksums: " + String(parser.failedChecksum()));
  XBeeInterface::send(p.toString(), xbee::MessageType::TELEMETRY);
}
void setup() {
  // For Debug
  Serial.begin(9600);

  sensors::initialize();
  xbee::XBeeInterface::initialize();

  sensors::reset();
  sensors::listenGPS();

  Timer1.initialize(1e4); // 10 ms = 10^4
  Timer1.attachInterrupt(listenSerials);
}

void loop() {
  makeOneLifecycle();
  auto start = millis();
  do
  {
    listenSerials();
    /* code */
  } while (millis() - start < 1000);
  
  delay(500);
}
