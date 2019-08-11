#ifndef STRING_TYPE
    #ifdef __GNUC__
        #include <string.h>
        #define STRING_TYPE std::string
    #else
        #define STRING_TYPE string
    #endif
#endif

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
    Packet (float temperature, float pressure, float voltage, float humidity);

    Packet (int id, float temperature, float pressure, float voltage, float humidity);

    Packet (const Packet& p);

    STRING_TYPE toString();

    static int getNextId();

    static void resetId ();

    int getId() { return id; }
};