#ifndef __XBEEINTERFACE__
#define __XBEEINTERFACE__

#include "xbeeinterface.hpp"
#include "commands.cpp"

#if IS_NOT_CONTROLLER
#include <iostream>
#else
#include <MD5.h>
#endif

namespace xbee {

#if IS_CONTROLLER
    SoftwareSerial * XBeeInterface::xbeeSerial = new SoftwareSerial(XBeeInterface::xbeeRX, XBeeInterface::xbeeTX);
#endif
    long XBeeInterface::listenStartTime = 0;
    bool XBeeInterface::canUseCommand = false;
    STRING_TYPE XBeeInterface::lastCommand = "";
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

    void XBeeInterface::send(const STRING_TYPE& msg, const MessageType& type) {
    #if IS_CONTROLLER
        // Send message through xbee if it is controller
        STRING_TYPE message = String((int)type) + "/" + msg;

        // Generate and concatenate hash

        unsigned char* hash = MD5::make_hash(const_cast<char*>(message.c_str()));
        char* r = MD5::make_digest(hash, 16);
        free(hash);
        message += "|" + String(r);
        free(r);
    
        xbeeSerial->println(message);
    #else
        std::cout << "Sending through xbee: " << msg << std::endl;
    #endif
    }

    void XBeeInterface::listen() {
    #if IS_CONTROLLER
        // Read xbee buffer
        static STRING_TYPE msg = "";
        while (xbeeSerial->available()) {
            char c = xbeeSerial->read();
            if (c == '\n') {
                // Action on command end
                // TODO: Change this to command handling
                if (msg != "") {
                    lastCommand = msg;
                    canUseCommand = true;
                    msg = "";
                }
            } else {
                msg += c;
            }
        }
    #endif
    }
}


#endif