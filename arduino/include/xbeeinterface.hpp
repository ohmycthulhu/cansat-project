#include "common.hpp"

// #if IS_CONTROLLER
// #include <SoftwareSerial.h>
// #endif

namespace xbee {
    enum class MessageType {
        TELEMETRY,
        COMMAND_REPORT
    };

    class XBeeInterface {
    private:
        constexpr static int xbeeRX = 6, xbeeTX = 7;
        constexpr static long listenTimeout = 1500; // micros
        static long listenStartTime;
    #if IS_CONTROLLER
        static SoftwareSerial * xbeeSerial;
    #endif
        static void setup();
    public:
        static void listen();
        static void initialize();
        static void send(const STRING_TYPE& msg, const MessageType& type);


        static bool isListeningXBee() { return xbeeSerial != nullptr && xbeeSerial->listen(); }
        static void listenXBee() {
            listenStartTime = millis();
            xbeeSerial->listen();
        }
        static bool shouldInterrupt(const long& time) {
            return isListeningXBee() && (time - listenStartTime > listenTimeout);
        }
    };

}