#ifndef __XBEEINTERFACE__
#define __XBEEINTERFACE__

#include "xbeeinterface.hpp"
#include "commands.cpp"

#if IS_NOT_CONTROLLER
#include <iostream>
#else
#include "md5.hpp"
//#include <MD5.h>
#endif

namespace xbee {

    void initialize() {
        xbeeSerial.begin(9600);
    }

    void send(const STRING_TYPE& msg, const MessageType& type) {
    #if IS_CONTROLLER
        // Send message through xbee if it is controller
        STRING_TYPE message = String((int)type) + "/" + msg;

        // Generate and concatenate hash

        // unsigned char* hash = MD5::make_hash((char*)message.c_str());
        // char* r = MD5::make_digest(hash, 16);
        // free(hash);
        // message += "|" + String(r);
        // free(r);
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
                // TODO: Change this to command handling
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