#ifndef __PACKET__
#define __PACKET__
#include "packet.hpp"
#include "md5.hpp"

#if IS_NOT_CONTROLLER
#include <sstream>
#endif

int Packet::nextId = 1;

Packet::Packet(const float temperature, const float pressure, const float voltage, const float humidity)
    : id(nextId), temperature(temperature), pressure (pressure), voltage(voltage), humidity(humidity)
{
    nextId++;
}

Packet::Packet(const int id, const float temperature, const float pressure, const float voltage, const float humidity)
    : id(id), temperature(temperature), pressure (pressure), voltage(voltage), humidity(humidity)
{}

Packet::Packet(const Packet& packet)
    : id(packet.id), temperature(packet.temperature), pressure (packet.pressure),
    voltage(packet.voltage), humidity(packet.humidity)
{}

void Packet::setID(const int& id) {
    nextId = id;
}

int Packet::getNextId() {
    return nextId;
}

STRING_TYPE Packet::toString() const {
#if IS_NOT_CONTROLLER
    auto result = std::stringstream();
    result  << id << "|"
            << temperature << "|"
            << pressure << "|"
            << humidity << "|"
            << voltage;
    auto s = result.str();
#else
    auto s = String(id) + "|" + String(temperature) + "|" + String(pressure) + "|" + String(humidity) + "|" + String(voltage);
#endif

    // Generate and concatenate hash
#if IS_NOT_CONTROLLER
    md5 hash(s.begin(), s.end());
    result << "|" << hash.hex_digest<std::string>();
    s = result.str();
#else
    md5 hash(s.c_str());
    s += "|" + hash.digest();
    // TODO: Write hash calculation and concatenating for Arduino
#endif
    return s;
}

#endif