#ifndef __XBEEINTERFACE__
#define __XBEEINTERFACE__

#include "xbeeinterface.hpp"

#if IS_NOT_CONTROLLER
#include <iostream>
#endif

namespace xbee {
    // QueueImplementation<STRING_TYPE> XBeeInterface::sendQueue;
    // QueueImplementation<STRING_TYPE> XBeeInterface::receivedCommands;

    void XBeeInterface::setup() {
    #if IS_CONTROLLER
        // TODO: Write setuping thread for listenning commands
    #endif
    }
    
    void XBeeInterface::initialize() {
        setup();
    }

    STRING_TYPE XBeeInterface::getCommand() {
    #if IS_CONTROLLER
        // Read string from serial
        return "";
    #else
        return "";
    #endif
    }
    
    void XBeeInterface::send(const STRING_TYPE& msg) {
        
    #if IS_CONTROLLER
        // Send message through xbee if it is controller
        Serial.println(msg);
    #else
        std::cout << "Sending through xbee: " << msg << std::endl;
    #endif
    }

    void XBeeInterface::listen() {
    #if IS_CONTROLLER
        // Listen for XBee.
        // Listenning should be done with this function as callback
    #endif
    }
}


#endif