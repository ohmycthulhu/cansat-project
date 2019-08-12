#ifndef __XBEEINTERFACE__
#define __XBEEINTERFACE__

#include "xbeeinterface.hpp"

#if IS_NOT_CONTROLLER
#include <iostream>
#endif

namespace xbee {
    std::queue<STRING_TYPE> XBeeInterface::sendQueue;
    std::queue<STRING_TYPE> XBeeInterface::receivedCommands;

    void XBeeInterface::setup() {
    #if IS_CONTROLLER
        // TODO: Write setuping thread for listenning commands
    #endif
    }
    
    void XBeeInterface::initialize() {
        setup();
    }

    STRING_TYPE XBeeInterface::getCommand() {
        auto result = receivedCommands.front();
        receivedCommands.pop();
        return result;
    }
    
    void XBeeInterface::send(const STRING_TYPE& msg) {
        sendQueue.push(msg);
    }

    void XBeeInterface::listen() {
    #if IS_CONTROLLER
        // Listen for XBee.
        // Listenning should be done with this function as callback
    #endif
    }

    void XBeeInterface::send() {
        if (!canSend()) {
            return;
        }
        STRING_TYPE s = sendQueue.front();
        sendQueue.pop();

    #if IS_CONTROLLER
        // Send message through xbee if it is controller
    #else
        std::cout << "Sending through xbee: " << s << std::endl;
    #endif
    }
}


#endif