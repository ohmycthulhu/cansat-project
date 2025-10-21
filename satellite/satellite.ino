#include <MemoryFree.h>
#include "include/sensors.cpp"
#include "include/xbeeinterface.cpp"

using namespace commands;
using namespace xbee;

// Set testing mode on
const bool TESTING_MODE = 1;

void setupTest();
void loopTest();

void makeOneLifecycle();
void smartDelay(long ms);
void beep(long duration, long frequency);

void setup() {
  if(TESTING_MODE) {
    setupTest();
  } else {
    sensors::initialize();
    xbee::initialize();
    sensors::listenGPS();
  }
}

void loop() {
  if(TESTING_MODE) {
    loopTest();
    sleep(700);
  } else {
    makeOneLifecycle();
    smartDelay(700);
  }
}

void makeOneLifecycle () {
  Commands executedCommand;
  if (xbee::isThereCommand()) {
    auto command = xbee::getCommand();
    auto status = commands::execute(command, &executedCommand);
    xbee::send(String((int)executedCommand) + "," + String((int)status), MessageType::COMMAND_REPORT);
    return;
  }
  Packet p = sensors::getPacket();
  auto commandStatus = commands::execute(p, &executedCommand);
  if (commandStatus != Statuses::NO_COMMAND) {
    xbee::send(String((int)executedCommand) + "," + String((int)commandStatus), xbee::MessageType::COMMAND_REPORT);
    return;
  }
  auto parser = sensors::getGPSParser();
  xbee::send(p.toString(), xbee::MessageType::TELEMETRY);
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

void beep(long duration, long frequency) {
  tone(sensors::buzzerPin, frequency);
  delay(duration);
  noTone(sensors::buzzerPin);
  delay(duration);
}

void setupTest() {
  Serial.begin(9600);

  /*
    Things that needs to be checked:
    Buzzer, XBee, BME, Light sensor, voltage divider, GPS, Camera
  */
  /*
    0. Short beep
    1. Initialize sensors and xbee
    2. Send message through xbee
    3. Little longer beep
    3.5 Initialize sensors
    4. Get data from BME and send through xbee
    5. Little longer beep
    6. Get value of light sensor and send
    7. Little longer beep
    8. Get value from voltage divider and send
    9. Little longer beep
    10. Get full packet and send
    11. Longer beep
    12. Start camera
    13. Little longer beep
    14. Start recording
    15. Wait for 5s
    16. Stop recording
    17. Little longer beep
    18. Very long beep
  */

  // Step #0
  beep(500, 400);

  // Step #1
  xbee::initialize();

  // Step #2
  xbee::send("XBee has been initialized");

  // Step #3
  beep(800, 500);

  // Step #3.5
  sensors::initialize();

  // Step #4
  float temp = sensors::bme != nullptr ? sensors::bme->readTemperature() : -1;
  float pressure = sensors::bme != nullptr ? sensors::bme->readPressure() : -1;
  float humidity = sensors::bme != nullptr ? sensors::bme->readHumidity() : -1;
  xbee::send("Data read from BME: " + String(temp) + ", " + String(pressure) + ", " + String(humidity));

  // Step #5
  beep(1000, 700);

  // Step #6
  auto lightSensorData = analogRead(sensors::lightSensor);
  xbee::send("Data read from Light Sensor: " + String(lightSensorData));

  // Step #7
  beep(1100, 900);

  // Step #8
  auto voltage = analogRead(sensors::voltageDividerPin) / 1024.0f * 10;
  xbee::send("Data read and transformed from Voltage Divider: " + String(voltage));

  // Step #9
  beep(1300, 1200);

  // Step #10
  auto packet = sensors::getPacket();
  xbee::send("Full packet: \n" + packet.toString());

  // Step #11
  beep(2000, 2000);

  // Step #12
  sensors::startCamera(true);
  xbee::send("Started camera...");

  // Step #13
  beep(1500, 800);

  // Step #14
  sensors::startRecording();
  xbee::send("Started recording...");
  beep(1000, 400);

  // Step #15
  delay(5000);

  // Step #16
  sensors::stopRecording();
  xbee::send("Stopped recording");

  // Step #17
  beep(3000, 1000);
}

void loopTest() {
  /*
  0. Listen for 5 second for GPS.
  1. Send info through XBee.
  2. Short beep
  3. Listen for 5 second XBee
  4. Execute command that got
  5. Long beep
*/
  // Steps #0, #1
  {
    // Listening is written in block, so can be easily deleted from code
    long start = millis();
    xbee::send("Starting listening GPS");
    while (millis() - start < 5000) {
      sensors::listen();
    }
    auto p = sensors::getPacket();
    xbee::send("Information got from GPS: " + String(p.latitude) + ", " + String(p.longitude) + ", " + p.gpsTime);
    xbee::send("Finished listening GPS");
  }

  // Step #2
  beep(500, 1000);

  // Step #3, #4
  {
    // Listening is written in block, so can be easily deleted from code
    long start = millis();
    xbee::send("Starting listening XBee");
    while (millis() - start < 5000) {
      xbee::listen();
      if (xbee::canUseCommand) {
        commands::Commands executedCommand;
        auto status = commands::execute(xbee::getCommand(), &executedCommand);
        xbee::send(String((int)executedCommand) + "," + String((int)status), xbee::MessageType::COMMAND_REPORT);
      }
    }
    xbee::send("Finished listening XBee");
  }

  // Step #5
  beep(2000, 500);
  xbee::send("Finished round");
}
