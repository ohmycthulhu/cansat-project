#include "common.hpp"

class Packet {
public:
	// Save next id for automatic id recognition
    static unsigned int nextId;
    unsigned int id;
    float temperature;
    float pressure;
    float voltage;
    float humidity;
    float height;
    float speed;
    float time;
    double latitude;
    double longitude;
    STRING_TYPE gpsTime;
    short satState;

    Packet (const float temperature, const float pressure, const float voltage,
            const float humidity, const float height, const float speed, const float time,
            const double latitude, const double longitude, STRING_TYPE gpsTime, const short state);

    Packet (const unsigned int id, const float temperature, const float pressure, const float voltage,
            const float humidity, const float height, const float speed, const float time,
            const double latitude, const double longitude, STRING_TYPE gpsTime, const short state);

    Packet (const Packet& p);

    STRING_TYPE toString() const;
};