#ifndef __SENSORS__
#define __SENSORS__

#include "sensors.hpp"
// #include "test.hpp"
#if IS_CONTROLLER
#include <EEPROM.h>
#else
#include <stdlib.h>
#endif

namespace sensors {

    void setupSensors() {
    #if IS_CONTROLLER
    /*
        This part will be written later
    */
        if (bme == nullptr) {
            bme = new Adafruit_BME280();
            bme->begin();
        }
        gpsSerial.begin(9600);
        pinMode(cameraMode, OUTPUT);
        pinMode(cameraPower, OUTPUT);
        pinMode(lightSensor, INPUT);
        // test::printInterface << "BME status is " << status << test::endOfLine;
    #endif
    }

    void initialize() {
        setupSensors();
        unsigned int id;
        EEPROM.get(packetIdAddress, id);
        if (id <= 0) {
            id = 1;
        }
        Packet::nextId = id;
        EEPROM.get(defaultPressureAddress, defaultPressure);
        EEPROM.get(baseTimeAddress, timeBase);
        int cameraState = EEPROM.read(cameraStateAddress);
        isPowered = cameraState > 0;
        isRecording = cameraState > 1;
        if (!isRecording) {
            startRecording();
        }
    }

    float getTime() {
    #if IS_CONTROLLER
        return timeBase + (millis() / 1e3);
    #else
        return 0;
    #endif
    }

    void reset() {
        unsigned int id = 1;
        defaultPressure = 0;
        timeBase = -(millis() / 1e3);
        EEPROM.put(packetIdAddress, id);
        EEPROM.put(defaultPressureAddress, defaultPressure);
        EEPROM.put(baseTimeAddress, 0);
        Packet::nextId = id;
    }

    void startCamera(const bool force) {
        if (force || !isPowered) {
            isPowered = true;
            isRecording = false;
            // Turning on camera
            digitalWrite(cameraPower, HIGH);
            delay(2500);
            digitalWrite(cameraPower, LOW);
            delay(700);
            /*
            // Toggling through 2 modes
            digitalWrite(cameraMode, HIGH);
            delay(500);
            digitalWrite(cameraMode, LOW);
            delay(700); */
            // No, toggle 1 mode
            digitalWrite(cameraMode, HIGH);
            delay(500);
            digitalWrite(cameraMode, LOW);
            delay(700);
            
            EEPROM.write(cameraStateAddress, 1); // 1 = camera is powered and don't record
        }
    }
    void startRecording() {
        if (!isPowered) {
            startCamera();
        }
        if (!isRecording) {
            // Power for .5 seconds
            digitalWrite(cameraPower, HIGH);
            delay(500);
            digitalWrite(cameraPower, LOW);
            EEPROM.write(cameraStateAddress, 2); // 2 = camera is powered and records
            isRecording = true;
        }
    }

    void stopRecording() {
        if (isPowered && isRecording) {
            // Power for .5 seconds
            digitalWrite(cameraPower, HIGH);
            delay(500);
            digitalWrite(cameraPower, LOW);
            EEPROM.write(cameraStateAddress, 0); // 1 = camera is powered and doesn't record
            isRecording = false;
            isPowered = false;
            // Stop camera
            digitalWrite(cameraPower, HIGH);
            delay(8000);
            digitalWrite(cameraPower, LOW);
        }
    }

    void startBuzzer() {
        tone(buzzerPin, 400);
    }
    void stopBuzzer() {
        noTone(buzzerPin);
    }

    void listen() {
        if (!isListeningGPS()) {
            listenGPS();
        }
        while (gpsSerial.available()){
            gpsParser.encode(gpsSerial.read());
        }
    }

    Packet getPacket() {
    #if IS_CONTROLLER
        float temp = bme != nullptr ? bme->readTemperature() : -1;
        float pressure = bme != nullptr ? bme->readPressure() : -1;
        float voltage = analogRead(voltageDividerPin) / 1024.0f * 10;
        float humidity = bme != nullptr ? bme->readHumidity() : -1;
        float height;
        if (bme == nullptr) {
            height = -1;
        }
        if (defaultPressure == 0) {
            defaultPressure = bme->readPressure() / 100.0f;
            EEPROM.put(defaultPressureAddress, defaultPressure);
        }
        height = bme->readAltitude(defaultPressure);
        float speed;
        if (prevTime == 0 || prevHeight == 0) {
            speed = 0;
        }
        auto dT = getTime() - prevTime;
        speed = abs(dT) > 1e-3 ? (height - prevHeight)/dT : 0;
        double latitude = gpsParser.location.lat();
        double longitude = gpsParser.location.lng();
        short satState = analogRead(lightSensor) > 512;
        STRING_TYPE gpsTime =  String(gpsParser.date.day()) + "-" +
        String(gpsParser.date.month()) + "-" +
        String(gpsParser.date.year()) + " " +
        String(gpsParser.time.hour()) + ":" +
        String(gpsParser.time.minute()) + ":" +
        String(gpsParser.time.second());
    #else
        float temp = std::rand() % 300 / 10.0f;
        float pressure = std::rand() % 1000 + 99500;
        float voltage = std::rand() % 20 / 10.0f + 3;
        float humidity = std::rand() % 1000 / 10.0f;
        float height = std::rand() % 1000 / 10.0f;
        double latitude = 0.0, longitude = 0.0;
        short satState = 0;
        STRING_TYPE gpsTime = "";
    #endif
        Packet packet = Packet(
            kalmanTemp.update(temp),
            kalmanPress.update(pressure),
            kalmanVoltage.update(voltage),
            kalmanHumidity.update(humidity),
            kalmanHeight.update(height),
            speed,
            getTime(),
            latitude,
            longitude,
            gpsTime,
            satState
        );
        prevTime = packet.time;
        prevHeight = height;
        EEPROM.put(packetIdAddress, packet.id);
        EEPROM.put(baseTimeAddress, packet.time);
        return packet;
    }

}

#endif