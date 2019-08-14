#include "packet.cpp"
#include "common.hpp"
#include "kalman.hpp"

namespace sensors {
    // Here goes sensors' pins
    const int dhtRx = 1;
    
    class Sensors {
    private:
        static Packet* lastPacket;
        static KalmanFilter<float> kalmanTemp, kalmanPress, kalmanHumidity, kalmanVoltage;

        static void setupSensors();
        static float getTemperature();
        static float getPressure();
        static float getHumidity();
        static float getVoltage();
    public:
        static void initialize();
        static Packet getPacket();
        static Packet* getLastPacket();
    };
}