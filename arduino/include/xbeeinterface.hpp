#include "common.hpp"

// #if IS_CONTROLLER
// #include <SoftwareSerial.h>
// #endif

namespace xbee {
    enum class MessageType {
        TELEMETRY,
        COMMAND_REPORT
    };


    constexpr static int xbeeRX = 6, xbeeTX = 7;
    constexpr static long listenTimeout = 1000; // micros
    long listenStartTime = 0;
    bool canUseCommand = false;
    STRING_TYPE lastCommand = "";    
    static void listen();
    static void initialize();
    static void send(const STRING_TYPE& msg, const MessageType& type);


    #if IS_CONTROLLER
        SoftwareSerial xbeeSerial(xbee::xbeeRX, xbee::xbeeTX);
    #endif

    static bool shouldInterrupt(const long& time) {
        return xbeeSerial.isListening() && (time - listenStartTime > listenTimeout);
    }

    static bool isThereCommand() { return canUseCommand; }
    static STRING_TYPE getCommand() { canUseCommand = false; return lastCommand; }

}