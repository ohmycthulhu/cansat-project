#include <iostream>
#include <stdlib.h>
#include "include/packet.cpp"
#include "include/sensors.cpp"

using namespace std;

namespace tests {
    bool checkPackets();
    bool checkSensors();
}

int main(int argc, char** argv) {
        cout << "Starting tests..." << endl;
        // Checking packets
        auto packetsState = tests::checkPackets();
        
        cout << "Packets state: " << (packetsState ? "work" : "error") << endl;
        // Checking sensors

        auto sensorsState = tests::checkSensors();
        
        cout << "Sensors state: " << (sensorsState ? "work" : "error") << endl;

        return 0;
    }

// Functions for testings
bool tests::checkPackets() {
    float temp = rand() % 300 / 10.0f;
    float press = rand() % 1000 + 99500.0f;
    float humidity = rand() % 1000 / 10.0f;
    float voltage = rand() % 10 / 5.0f + 3;

    // Check automatic id increase
    for (int index = 1; index < 100; index++) {
        Packet p1 = Packet(temp, press, voltage, humidity);
        if (index != p1.getId()) {
            return false;
        }
    }

    return true;
}

bool tests::checkSensors() {
    using namespace sensors;
    try {
        Sensors::initialize();
        Packet p1 = Sensors::getPacket();
        Packet* p2 = Sensors::getLastPacket();
        if (p1 != *p2) {
            return false;
        }
        for (int i = 0; i < 10; i++) {
            Packet temp = Sensors::getPacket();
            cout << "Packet #" << i << " :" << temp.toString() << endl;
        }
    } catch(exception e) {
        return false;
    }
    return true;
}