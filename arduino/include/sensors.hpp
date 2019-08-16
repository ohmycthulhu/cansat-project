#if IS_CONTROLLER
#include <Adafruit_BME280.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
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
        static SoftwareSerial * gpsSerial;
        static TinyGPSPlus gpsParser;
    #endif
       /*
            </Sensors>
       */

        /*
            <Pins>
        */
        const static int gpsTX = 4;
        const static int gpsRX = 5;
       /*
            </Pins>
        */

        constexpr static int listenTimeout = 2000; // millis
        constexpr static int packetIdAddress = 0x53;
        constexpr static int defaultPressureAddress = 0x63;
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
        static double getLongitude();
        static double getLatitude();
        static STRING_TYPE getGpsTime();
        static float getSpeed(const float& height);
    public:
        static void initialize();
        static void listen();
        static Packet getPacket();
        static Packet* getLastPacket();

        static void reset();
    };
}