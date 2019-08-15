#ifndef __SENSORS__
#define __SENSORS__

#include "sensors.hpp"
// #include "test.hpp"
#if IS_CONTROLLER
#else
#include <stdlib.h>
#endif

namespace sensors {

    Packet* Sensors::lastPacket = nullptr;
    KalmanFilter<float> Sensors::kalmanTemp, Sensors::kalmanPress, Sensors::kalmanHeight;
    KalmanFilter<float> Sensors::kalmanHumidity, Sensors::kalmanVoltage;
    float Sensors::defaultPressure = 0;
#if IS_CONTROLLER
    Adafruit_BME280 * Sensors::bme = nullptr;
#endif


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
        // test::printInterface << "BME status is " << status << test::endOfLine;
    #endif
    }

    void Sensors::initialize() {
        setupSensors();
    }

    float Sensors::getTime() {
    #if IS_CONTROLLER
        return millis() / 1e3;
    #else
        return 0;
    #endif
    }

    float Sensors::getSpeed(const float& height) {
        if (lastPacket == nullptr) {
            return 0;
        }
        float dT = getTime() - lastPacket->getTime();
        return abs(dT) > 1e-3 ? (height - lastPacket->getHeight())/dT : 0;
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
            getTime()
        );
        if (lastPacket != nullptr) {
            *lastPacket = Packet(packet);
        } else {
            lastPacket = new Packet(packet);
        }
        return packet;
    }

    Packet* Sensors::getLastPacket() {
        return lastPacket;
    }
}

#endif