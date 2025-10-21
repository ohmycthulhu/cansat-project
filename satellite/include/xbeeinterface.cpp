#ifndef __XBEEINTERFACE__
#define __XBEEINTERFACE__

#include "xbeeinterface.hpp"
#include "commands.cpp"

#if IS_NOT_CONTROLLER
#include <iostream>
#else
#include "hash.hpp"
#endif

namespace xbee {

    void initialize() {
        xbeeSerial.begin(9600);
    }

    void send(const STRING_TYPE& msg, const MessageType& type) {
    #if IS_CONTROLLER
        // Send message through xbee if it is a controller
        STRING_TYPE message = String((int)type) + "/" + msg;

        // Generate and concatenate hash
        message += "|" + String(simple_hash(message));
    
        xbeeSerial.println(message);
    #else
        std::cout << "Sending through xbee: " << msg << std::endl;
    #endif
    }

    void listen() {
    #if IS_CONTROLLER
        if (xbeeSerial.isListening()) {
            listenStartTime = millis();
            xbeeSerial.listen();
        }
        // Read xbee buffer
        static STRING_TYPE msg = "";
        STRING_TYPE s = "";
        while (xbeeSerial.available()) {
            int c = xbeeSerial.read();
            if (c == '\n') {
                // Action on command end
                if (msg != "") {
                    lastCommand = msg;
                    canUseCommand = true;
                    msg = "";
                    break;
                }
            } else {
                msg += (char)c;
            }
        }
    #endif
    }
}


#endif