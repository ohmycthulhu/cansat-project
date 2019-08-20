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

    KalmanFilter<float> Sensors::kalmanTemp, Sensors::kalmanPress, Sensors::kalmanHeight;
    KalmanFilter<float> Sensors::kalmanHumidity, Sensors::kalmanVoltage;
    float Sensors::prevTime = 0, Sensors::prevHeight = 0;
    // double Sensors::lat = 0, Sensors::lng = 0;
    float Sensors::defaultPressure = 0;
    float Sensors::timeBase = 0.0f;
#if IS_CONTROLLER
    Adafruit_BME280 * Sensors::bme = nullptr;
    SoftwareSerial * Sensors::gpsSerial = new SoftwareSerial(Sensors::gpsRX, Sensors::gpsTX);
    TinyGPSPlus Sensors::gpsParser;
#endif
    long Sensors::listenStartTime = 0;

    bool Sensors::isPowered = false, Sensors::isRecording = false;

    float Sensors::getTemperature() {
    #if IS_CONTROLLER
        /*
            This part will be written when I get access to sensors
        */
       return bme != nullptr ? bme->readTemperature() : -1;
    #else
       return std::rand() % 300 / 10.0f;
    #endif
    }

    short Sensors::getSatelliteState() {
    #if IS_CONTROLLER
        return analogRead(lightSensor) > 128;
    #endif
        return 0;
    }

    float Sensors::getVoltage() {
    #if IS_CONTROLLER
        /*
            This part will be written when I get access to sensors
        */
    #else
       return std::rand() % 20 / 10.0f + 3;
    #endif
    }

    float Sensors::getPressure() {
    #if IS_CONTROLLER
        /*
            This part will be written when I get access to sensors
        */
        return bme != nullptr ? bme->readPressure() : -1;
    #else
       return std::rand() % 1000 + 99500;
    #endif
    }

    float Sensors::getHumidity() {
    #if IS_CONTROLLER
        /*
            This part will be written when I get access to sensors
        */
        return bme != nullptr ? bme->readHumidity() : -1;
    #else
       return std::rand() % 1000 / 10.0f;
    #endif
    }

    float Sensors::getHeight() {
    #if IS_CONTROLLER
        // If default pressure is not set, then find by BME
        if (bme == nullptr) {
            return -1;
        }
        if (defaultPressure == 0) {
            defaultPressure = bme->readPressure() / 100.0f;
            EEPROM.put(defaultPressureAddress, defaultPressure);
        }
        return bme->readAltitude(defaultPressure);
    #else
        return std::rand() % 1000 / 10.0f;
    #endif
    }

    void Sensors::setupSensors() {
    #if IS_CONTROLLER
    /*
        This part will be written later
    */
        if (bme == nullptr) {
            bme = new Adafruit_BME280();
            bme->begin();
        }
        if (gpsSerial != nullptr) {
            gpsSerial->begin(9600);
        }
        pinMode(cameraMode, OUTPUT);
        pinMode(cameraPower, OUTPUT);
        pinMode(lightSensor, INPUT);
        // test::printInterface << "BME status is " << status << test::endOfLine;
    #endif
    }

    void Sensors::initialize() {
        setupSensors();
        unsigned int id;
        EEPROM.get(packetIdAddress, id);
        if (id <= 0) {
            id = 1;
        }
        Packet::setID(id);
        EEPROM.get(defaultPressureAddress, defaultPressure);
        EEPROM.get(baseTimeAddress, timeBase);
        int cameraState = EEPROM.read(cameraStateAddress);
        isPowered = cameraState > 0;
        isRecording = cameraState > 1;
    }

    float Sensors::getTime() {
    #if IS_CONTROLLER
        return timeBase + (millis() / 1e3);
    #else
        return 0;
    #endif
    }

    double Sensors::getLongitude() {
    #if IS_CONTROLLER
        // return lng;
        return 0;
        return gpsParser.location.lng();
    #else
        return 0;
    #endif
    }

    double Sensors::getLatitude() {
    #if IS_CONTROLLER
        // return lat;
        return 0;
   //     Serial.println(gpsParser.location.lat());
 //       return gpsParser.location.lat();
    #else
        return 0;
    #endif
    }

    STRING_TYPE Sensors::getGpsTime() {
    #if IS_CONTROLLER
    // Time format - HH:mm:ss
        return "";
        return String(gpsParser.time.hour()) + ":" 
            + String(gpsParser.time.minute()) + ":"
            + String(gpsParser.time.second());
    #else
        return "";
    #endif
    }

    float Sensors::getSpeed(const float& height) {
        if (prevTime == 0 || prevHeight == 0) {
            return 0;
        }
        float dT = getTime() - prevTime;
        return abs(dT) > 1e-3 ? (height - prevHeight)/dT : 0;
    }

    void Sensors::listen() {
    #if IS_CONTROLLER
        // Listen for GPS.
        while (gpsSerial->available()) {
            // Serial.println(gpsSerial->available());
            gpsParser.encode(gpsSerial->read());
        }
        // Serial.println(gpsParser.location.lat());
    #endif
    }

    void Sensors::reset() {
        unsigned int id = 1;
        defaultPressure = 0;
        timeBase = 0;
        EEPROM.put(packetIdAddress, id);
        EEPROM.put(defaultPressureAddress, defaultPressure);
        EEPROM.put(baseTimeAddress, timeBase);
        Packet::setID(id);
    }

    void Sensors::startCamera(const bool force) {
        if (force || !isPowered) {
            isPowered = true;
            isRecording = false;
            // Turning on camera
            digitalWrite(cameraPower, HIGH);
            delay(3000);
            digitalWrite(cameraPower, LOW);
            delay(700);
            // Toggling through 2 modes
            digitalWrite(cameraMode, HIGH);
            delay(700);
            digitalWrite(cameraMode, LOW);
            delay(700);
            digitalWrite(cameraMode, HIGH);
            delay(700);
            digitalWrite(cameraMode, LOW);
            delay(700);
            EEPROM.write(cameraStateAddress, 1); // 1 = camera is powered and don't record
        }
    }
    void Sensors::startRecording() {
        if (isPowered && !isRecording) {
            // Power for .7 seconds
            digitalWrite(cameraPower, HIGH);
            delay(700);
            digitalWrite(cameraPower, LOW);
            EEPROM.write(cameraStateAddress, 2); // 2 = camera is powered and records
        }
    }
    void Sensors::stopRecording() {
        if (isPowered && isRecording) {
            // Power for .7 seconds
            digitalWrite(cameraPower, HIGH);
            delay(700);
            digitalWrite(cameraPower, LOW);
            EEPROM.write(cameraStateAddress, 1); // 1 = camera is powered and doesn't record
        }
    }

    void Sensors::startBuzzer() {
        tone(buzzerPin, 400);
    }
    void Sensors::stopBuzzer() {
        noTone(buzzerPin);
    }

    Packet Sensors::getPacket() {
        auto height = kalmanHeight.update(getHeight());
        Packet packet = Packet(
            kalmanTemp.update(getTemperature()),
            kalmanPress.update(getPressure()),
            kalmanVoltage.update(getVoltage()),
            kalmanHumidity.update(getHumidity()),
            kalmanHeight.update(getHeight()),
            getSpeed(height),
            getTime(),
            getLatitude(),
            getLongitude(),
            getGpsTime(),
            getSatelliteState()
        );
        prevTime = packet.getTime();
        prevHeight = height;
        EEPROM.put(packetIdAddress, packet.getId());
        EEPROM.put(baseTimeAddress, packet.getTime());
        return packet;
    }

}

#endif