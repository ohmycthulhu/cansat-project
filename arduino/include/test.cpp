#ifndef __TESTS__
#define __TESTS__
#include "test.hpp"
#include "packet.cpp"
#include "sensors.cpp"
#include "md5.hpp"
#include "commands.cpp"
#include "xbeeinterface.cpp"
#include "kalman.hpp"

#if IS_CONTROLLER
#include <MemoryFree.h>
#endif

using namespace std;

namespace test {

    bool test() {

        Packet::setID(1);

        printInterface << "Starting tests..." << endOfLine;
        printInterface << endOfLine << "*****************************" << endOfLine << endOfLine;
        
        long startMemory = getFreeMemory();
        printInterface << "Initial free memory: " << startMemory << endOfLine;
        // Test hashing function
        auto measureBegin = getTime();
        auto hashState = testHash();
        auto measureEnd = getTime();
        long currentMemory = getFreeMemory();
        printInterface << "Hash state: " << (hashState ? "work" : "error") << endOfLine;
        printInterface << "Elapsed time: " << (measureEnd - measureBegin) << endOfLine;
        printInterface << "Memory change: " << (startMemory - currentMemory) << ". Memory left: " << currentMemory << endOfLine;
        printInterface << endOfLine << "*****************************" << endOfLine << endOfLine;

        // Checking sensors
        currentMemory = getFreeMemory();
        measureBegin = getTime();
        auto sensorsState = checkSensors();
        measureEnd = getTime();
        printInterface << "Sensors state: " << (sensorsState ? "work" : "error") << endOfLine;
        printInterface << "Elapsed time: " << (measureEnd - measureBegin) << endOfLine;
        printInterface << "Memory change: " << (currentMemory - getFreeMemory()) << ". Memory left: " << getFreeMemory() << endOfLine;
        printInterface << endOfLine << "*****************************" << endOfLine << endOfLine;

        // Test commands
        currentMemory = getFreeMemory();
        measureBegin = getTime();
        auto commandsState = testCommands();
        measureEnd = getTime();
        printInterface << "Commands state: " << (commandsState ? "work" : "error") << endOfLine; 
        printInterface << "Elapsed time: " << (measureEnd - measureBegin) << endOfLine;
        printInterface << "Memory change: " << (currentMemory - getFreeMemory()) << ". Memory left: " << getFreeMemory() << endOfLine;
        printInterface << endOfLine << "*****************************" << endOfLine << endOfLine;

        // Test Xbee
        currentMemory = getFreeMemory();
        measureBegin = getTime();
        auto xbeeState = testXbee();
        measureEnd = getTime();
        printInterface << "Xbee state: " << (xbeeState ? "work" : "error") << endOfLine; 
        printInterface << "Elapsed time: " << (measureEnd - measureBegin) << endOfLine;
        printInterface << "Memory change: " << (currentMemory - getFreeMemory()) << ". Memory left: " << getFreeMemory() << endOfLine;
        printInterface << endOfLine << "*****************************" << endOfLine << endOfLine;
        
        return commandsState && hashState && xbeeState && sensorsState;
    }

    /*
    // Functions for testings
    bool checkPackets() {
        double temp = randomNumber(30);
        double press = randomNumber(10500, 99500);
        double humidity = randomNumber(100);
        double voltage = randomNumber(5, 3);
        double height = randomNumber(100, 10);
        double time = getTime();
        double speed = randomNumber(5, 3);

        // Check automatic id increase
        for (int index = 1; index < 100; index++) {
            Packet p1 = Packet(temp, press, voltage, humidity, height, speed, time);
            if (index != p1.getId()) {
                return false;
            }
        }

        return true;
    }
    */

    bool checkSensors() {
        using namespace sensors;
        // There is no try-catch in Arduino, so we hide this block for it
        #if IF_NOT_CONTROLLER
        try {
        #endif
            Sensors::initialize();
            Packet p1 = Sensors::getPacket();
            Packet* p2 = Sensors::getLastPacket();
            
            if (p1 != *p2) {
                return false;
            }
            printInterface << "Size of packet is " << sizeof(*p2) << endOfLine;
            for (int i = 0; i < 20; i++) {
                Packet temp = Sensors::getPacket();
                printInterface << "Packet #" << i << " :" << temp.toString() << endOfLine;
            }
        #if IF_NOT_CONTROLLER
        } catch(Exception p) {
            return false;
        }
        #endif
        return true;
    }

    bool testHash() {
        STRING_TYPE strings[] = {
            "First", "Second", "Third" , "Fourth"
        };
        STRING_TYPE hashes[] = {
            "7fb55ed0b7a30342ba6da306428cae04", "c22cf8376b1893dcfcef0649fe1a7d87",
            "168909c0b6f1dfbd48f679d47059c1d6" , "6e599f7a2a9186d391be4537f105be98"
        };

        STRING_TYPE hashResult;
        STRING_TYPE initialText;

        for (int i = 0; i < sizeof(hashes) / sizeof(STRING_TYPE); i++) {
            initialText = strings[i];
        #if IS_CONTROLLER
            // md5 h(strings[i].c_str());
            // hashResult = h.digest();
            hashResult = md5::digest(strings[i]);
        #else
            md5 h(strings[i].begin(), strings[i].end());
            hashResult = h.hex_digest<STRING_TYPE>();
        #endif
            printInterface << initialText << "|" << hashResult << "|" << hashes[i] << endOfLine;
            if (hashes[i] != hashResult) {
                return false;
            }
        }

        return true;
    }

    bool testCommands() {
        // TODO: Write tests to check commands
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
            result = commands::CommandsInterface::execute(messages[i]);
            printInterface << "Result of executing ("<< messages[i] <<") is "
                << (int)result << " while should be " << (int)statuses[i] << endOfLine;
            if (statuses[i] != result) {
                return false;
            }
        }

        return true;
    }

    bool testXbee() {
        xbee::XBeeInterface::initialize();
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
            xbee::XBeeInterface::send(s[i]);
        }

        return true;
    }

    /*
    bool testKalman() {
        KalmanFilter<double> filter;

        double value = randomNumber(100);
        filter.update(value);
        printInterface << "Initial value is " << filter.getValue() << endOfLine;
        for (int i = 0; i < 10; i++) {
            value = randomNumber(100);
            printInterface << "Value is " << filter.update(value) << " while actual value is " << value << endOfLine;
        }

        printInterface << "Estimating to the value: " << value << endOfLine;    
        for (int i = 0; i < 10; i++) {
            printInterface << "Value is " << filter.update(value) << endOfLine;    
        }
        return true;
    }
    */

    #if IS_NOT_CONTROLLER
    long getFreeMemory() {
        // Function to get free memory on Arduino
        return 0;
    }
    #endif
}

#endif