#include "common.hpp"

class Packet {
private:
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

public:
    Packet (const float temperature, const float pressure, const float voltage,
            const float humidity, const float height, const float speed, const float time,
            const double latitude, const double longitude, STRING_TYPE gpsTime, const short state);

    Packet (const unsigned int id, const float temperature, const float pressure, const float voltage,
            const float humidity, const float height, const float speed, const float time,
            const double latitude, const double longitude, STRING_TYPE gpsTime, const short state);

    Packet (const Packet& p);

    STRING_TYPE toString() const;

    static unsigned int getNextId() { return nextId; }

    static void setID (const unsigned int& id = 1) { nextId = id; }

    float getHeight() const { return height; }
    float getTime() const { return time; }
    short getState() const { return satState; }
    unsigned int getId() const { return id; }

};