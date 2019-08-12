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

public:
    Packet (const float temperature, const float pressure, const float voltage, const float humidity);

    Packet (const int id, const float temperature, const float pressure, const float voltage, const float humidity);

    Packet (const Packet& p);

    STRING_TYPE toString() const;

    static int getNextId();

    static void resetId ();

    int getId() const { return id; }

    bool operator==(const Packet& p) const { return id == p.id; }
    bool operator!=(const Packet& p) const { return id != p.id; }
    bool operator<(const Packet& p) const { return id < p.id; }
    bool operator>(const Packet& p) const { return id > p.id; }
};