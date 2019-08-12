#include <queue>
#include "common.hpp"

namespace xbee {
    class XBeeInterface {
    public:
        static std::queue<STRING_TYPE> receivedCommands;
        static std::queue<STRING_TYPE> sendQueue;
        static void setup(); 
        static void listen();
        static void send();
        static bool canSend() { return !sendQueue.empty(); }
    public:
        static void initialize();
        static bool isThereCommand() { return !receivedCommands.empty(); }
        static STRING_TYPE getCommand();
        static void send(const STRING_TYPE& msg);
    };
}