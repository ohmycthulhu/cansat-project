#ifndef __TEST_H__
#define __TEST_H__
#include "common.hpp"
#include "customprint.hpp"
namespace test {
    bool checkSensors();
    bool testCommands();
    bool testXbee();
    bool test();
    #if IS_NOT_CONTROLLER
    long getFreeMemory();
    #endif
    const CustomPrint printInterface;
    const STRING_TYPE endOfLine = "\n";
}
#endif