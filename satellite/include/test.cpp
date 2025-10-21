#ifndef __TESTS__
#define __TESTS__
#include "test.hpp"
#include "packet.cpp"
#include "sensors.cpp"
#include "commands.cpp"
#include "xbee::.cpp"
#include "kalman.hpp"

#if IS_CONTROLLER
#include <MemoryFree.h>
#endif

using namespace std;

namespace test {

    bool test() {

        Packet::nextId = 1;

        printInterface << "Starting tests..." << endOfLine;
        printInterface << endOfLine << "*****************************" << endOfLine << endOfLine;
        
        long startMemory = getFreeMemory();
        printInterface << "Initial free memory: " << startMemory << endOfLine;
  
        // Checking sensors
        auto currentMemory = getFreeMemory();
        auto measureBegin = getTime();
        auto sensorsState = checkSensors();
        auto measureEnd = getTime();
        printInterface << "Sensors state: " << (sensorsState ? "work" : "error") << endOfLine;
        printInterface << "Elapsed time: " << (measureEnd - measureBegin) << endOfLine;
        printInterface << "Memory change: " << (currentMemory - getFreeMemory()) << ". Memory left: " << getFreeMemory() << endOfLine;
        printInterface << endOfLine << "*****************************" << endOfLine << endOfLine;

        xbee::listen();
        sensors::listen();

        return true;
    }

    bool checkSensors() {
        // There is no try-catch in Arduino, so we hide this block for it
        #if IF_NOT_CONTROLLER
        try {
        #endif
            sensors::initialize();
            
            for (int i = 0; i < 20; i++) {
                Packet temp = sensors::getPacket();
                printInterface << "Packet #" << i << " :" << temp.toString() << endOfLine;
            }
        #if IF_NOT_CONTROLLER
        } catch(Exception p) {
            return false;
        }
        #endif
        return true;
    }

    bool testCommands() {
        STRING_TYPE messages[] =  {
            "1|c4ca4238a0b923820dcc509a6f75849b",
            "2|[doaspdash",
            "1|dsadsadas[pk",
            "flaslkjjkasn",
            "1|1|[pwiqehq",
            "12|c20ad4d76fe97759aa27a0c99bff6710"
        };
        commands::Statuses statuses[] = {
            commands::Statuses::OK,
            commands::Statuses::HASH_FAILED,
            commands::Statuses::HASH_FAILED, 
            commands::Statuses::HASH_FAILED, 
            commands::Statuses::HASH_FAILED,  
            commands::Statuses::NO_COMMAND
        };
        
        commands::Statuses result;
        for (int i = 0; i < sizeof(statuses) / sizeof(commands::Statuses); i++) {
            result = commands::execute(messages[i]);
            printInterface << "Result of executing ("<< messages[i] <<") is "
                << (int)result << " while should be " << (int)statuses[i] << endOfLine;
            if (statuses[i] != result) {
                return false;
            }
        }

        return true;
    }

    bool testXbee() {
        xbee::initialize();
        STRING_TYPE s[] = {
            "First",
            "Second",
            "Fourth",
            "Eighth",
            "Sixteenth",
            "Thirty second",
            "Sixty fourth"
        };

        for (int i = 0; i < sizeof(s) / sizeof(STRING_TYPE); i++) {
            xbee::send(s[i], xbee::MessageType::TELEMETRY);
        }

        return true;
    }

    #if IS_NOT_CONTROLLER
    long getFreeMemory() {
        return 0;
    }
    #endif
}

#endif