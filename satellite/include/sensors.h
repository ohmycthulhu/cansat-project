#include "packet.cpp"
#include "common.hpp"
#include "kalman.hpp"
#if IS_CONTROLLER
#include <Adafruit_BME280.h>
#include <TinyGPS++.h>
#endif

// Sensors were rewritten in C style to reduce program storage usage

namespace sensors {
    /*
        <Pins>
    */
    constexpr  uint8_t gpsTX = 4, gpsRX = 3;
    constexpr  uint8_t cameraMode = 2, cameraPower = 8;
    constexpr  uint8_t lightSensor = A1;
    constexpr  uint8_t buzzerPin = 5;
    constexpr uint8_t voltageDividerPin = A0;
    /*
        </Pins>
    */

    /*
        <Sensors>
    */
    #if IS_CONTROLLER
    // BME sensor for measuring temperature, pressure, height and humidity
        Adafruit_BME280* bme = nullptr;
        SoftwareSerial gpsSerial(gpsRX, gpsTX);
        TinyGPSPlus gpsParser;
    #endif
    /*
        </Sensors>
    */

    constexpr  long listenTimeout = 2000; // seconds
    constexpr  int packetIdAddress = 0x53;
    constexpr  int defaultPressureAddress = 0x63;
    constexpr  int cameraStateAddress = 0x83;
    constexpr  int baseTimeAddress = 0x93;
    float timeBase = 0.0f;
    KalmanFilter<float> kalmanTemp, kalmanPress, kalmanHumidity, kalmanVoltage, kalmanHeight;

	// Default pressure for calculating height depending on pressure difference
    float defaultPressure = 0;
    float prevTime = 0, prevHeight = 0;
    long listenStartTime = 0;

	// Camera information
    bool isRecording = false;
    bool isPowered = false;
    void setupSensors();
    float getTime();
    void initialize();
    void listen();
    Packet getPacket();

    void reset();

    bool isListeningGPS() { return gpsSerial.isListening(); }
    void listenGPS() {
        listenStartTime = millis();
        gpsSerial.listen();
    }
    bool shouldInterrupt(const long& time) {
        return isListeningGPS() && (time - listenStartTime > listenTimeout);
    }

    void startCamera (const bool force = false);
    void startRecording ();
    void stopRecording ();

    void startBuzzer();
    void stopBuzzer();

    const TinyGPSPlus& getGPSParser() { return gpsParser; }
}
