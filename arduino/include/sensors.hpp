#if IS_CONTROLLER
#include <Adafruit_BME280.h>
#endif
#include "packet.cpp"
#include "common.hpp"
#include "kalman.hpp"

namespace sensors {
    // Here goes sensors' pins
    const int dhtRx = 1;
    
    class Sensors {
    private:
        /*
            <Sensors>
        */
    #if IS_CONTROLLER
        // BME sensor for measuring temperature, pressure, height and humidity
        static Adafruit_BME280* bme;
    #endif
       /*
            </Sensors>
       */
        
        static Packet* lastPacket;
        static KalmanFilter<float> kalmanTemp, kalmanPress, kalmanHumidity, kalmanVoltage, kalmanHeight;
        // Default pressure for calculating height depending on pressure difference
        static float defaultPressure;
        static void setupSensors();
        static float getTemperature();
        static float getPressure();
        static float getHumidity();
        static float getHeight();
        static float getVoltage();
        static float getTime();
        static float getSpeed(const float& height);
    public:
        static void initialize();
        static Packet getPacket();
        static Packet* getLastPacket();
    };
}