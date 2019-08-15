#include "common.hpp"

namespace xbee {
    class XBeeInterface {
    public:
        static void setup(); 
        static void listen();
    public:
        static void initialize();
        static STRING_TYPE getCommand();
        static void send(const STRING_TYPE& msg);
    };
}