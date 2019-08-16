#ifndef __XBEEINTERFACE__
#define __XBEEINTERFACE__

#include "xbeeinterface.hpp"

#if IS_NOT_CONTROLLER
#include <iostream>
#endif

namespace xbee {

#if IS_CONTROLLER
    SoftwareSerial * XBeeInterface::xbeeSerial = new SoftwareSerial(XBeeInterface::xbeeRX, XBeeInterface::xbeeTX);
#endif

    void XBeeInterface::setup() {
    #if IS_CONTROLLER
        if (xbeeSerial != nullptr) {
            xbeeSerial->begin(9600);
        }
        // TODO: Write setuping thread for listenning commands
    #endif
    }
    
    void XBeeInterface::initialize() {
        setup();
    }

    void XBeeInterface::send(const STRING_TYPE& msg) {
        
    #if IS_CONTROLLER
        // Send message through xbee if it is controller
        xbeeSerial->println(msg);
    #else
        std::cout << "Sending through xbee: " << msg << std::endl;
    #endif
    }

    void XBeeInterface::listen() {
    #if IS_CONTROLLER
        auto startTime = getTime();
        xbeeSerial->listen();
        STRING_TYPE msg = "";
        while (getTime() - startTime < listenTimeout) {
            while (xbeeSerial->available()) {
                char c = xbeeSerial->read();
                if (c == '\n') {
                    // Action on command end
                    // TODO: Change this to command handling
                    test::printInterface << "Got command: " << msg << test::endOfLine;
                    msg = "";
                } else {
                    msg += c;
                }
            }
        }
        // Listen for XBee.
    #endif
    }
}


#endif