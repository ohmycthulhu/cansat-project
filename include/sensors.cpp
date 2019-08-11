#ifndef __SENSORS__
#define __SENSORS__

#include "sensors.hpp"

#if IS_NOT_CONTROLLER
#include <stdlib.h>
#endif

namespace sensors {

    Packet* Sensors::lastPacket = nullptr;

    float Sensors::getTemperature() {
    #if IS_CONTROLLER
        /*
            This part will be written when I get access to sensors
        */
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
    #else
       return std::rand() % 1000 + 99500;
    #endif
    }

    float Sensors::getHumidity() {
    #if IS_CONTROLLER
        /*
            This part will be written when I get access to sensors
        */
    #else
       return std::rand() % 1000 / 10.0f;
    #endif
    }

    void Sensors::setupSensors() {
    #if IS_CONTROLLER
    /*
        This part will be written later
    */
    #endif
    }

    void Sensors::initialize() {
        setupSensors();
    }

    Packet Sensors::getPacket() {
        Packet packet = Packet(getTemperature(), getPressure(), getVoltage(), getHumidity());
        if (lastPacket != nullptr) {
            delete lastPacket; 
        }
        lastPacket = new Packet(packet);
        return packet;
    }

    Packet* Sensors::getLastPacket() {
        return lastPacket;
    }
}

#endif