#include "packet.cpp"
#include "common.hpp"
#include "kalman.hpp"
#if IS_CONTROLLER
#include <Adafruit_BME280.h>
#include <TinyGPS++.h>
#endif

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

        constexpr static long listenTimeout = 2000; // seconds
        constexpr static int packetIdAddress = 0x53;
        constexpr static int defaultPressureAddress = 0x63;
        static KalmanFilter<float> kalmanTemp, kalmanPress, kalmanHumidity, kalmanVoltage, kalmanHeight;
        // Default pressure for calculating height depending on pressure difference
        static float defaultPressure;
        static float prevTime, prevHeight;
        static long listenStartTime;
        // static double lat, lng;
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

        static void reset();

        static bool isListeningGPS() { return gpsSerial != nullptr && gpsSerial->listen(); }
        static void listenGPS() {
            listenStartTime = millis();
            gpsSerial->listen();
        }
        static bool shouldInterrupt(const long& time) {
            return isListeningGPS() && (time - listenStartTime > listenTimeout);
        }
    };
}