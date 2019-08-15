#include "common.hpp"

class Packet {
private:
// Save next id for automatic id recognition
    static int nextId;
    int id;
    float temperature;
    float pressure;
    float voltage;
    float humidity;
    float height;
    float speed;
    float time;

public:
    Packet (const float temperature, const float pressure, const float voltage,
            const float humidity, const float height, const float speed, const float time);

    Packet (const int id, const float temperature, const float pressure, const float voltage,
            const float humidity, const float height, const float speed, const float time);

    Packet (const Packet& p);

    STRING_TYPE toString() const;

    static int getNextId();

    static void setID (const int& id = 1);

    float getHeight() const { return height; }
    float getTime() const { return time; }
    int getId() const { return id; }

    bool operator==(const Packet& p) const { return id == p.id; }
    bool operator!=(const Packet& p) const { return id != p.id; }
    bool operator<(const Packet& p) const { return id < p.id; }
    bool operator>(const Packet& p) const { return id > p.id; }
};