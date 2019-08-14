#include "common.hpp"
#include "customprint.hpp"
namespace test {
    bool checkPackets();
    bool checkSensors();
    bool testHash();
    bool testCommands();
    bool testXbee();
    bool testKalman();
    bool test();
    #if IS_NOT_CONTROLLER
    long getFreeMemory();
    #endif
    const CustomPrint printInterface;
    const STRING_TYPE endOfLine = "\n";
}