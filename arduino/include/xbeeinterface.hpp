#include "common.hpp"

#if IS_CONTROLLER
#include <SoftwareSerial.h>
#endif

namespace xbee {
    class XBeeInterface {
    private:
        constexpr static int xbeeRX = 6, xbeeTX = 7;
        constexpr static int listenTimeout = 1500; // millis
    #if IS_CONTROLLER
        static SoftwareSerial * xbeeSerial;
    #endif
        static void setup();
    public:
        static void listen();
        static void initialize();
        static void send(const STRING_TYPE& msg);
    };
}