#ifndef __PACKET__
#define __PACKET__
#include "packet.hpp"
#include <sstream>

int Packet::nextId = 1;

Packet::Packet(float temperature, float pressure, float voltage, float humidity)
    : id(nextId), temperature(temperature), pressure (pressure), voltage(voltage), humidity(humidity)
{
    nextId++;
}

Packet::Packet(int id, float temperature, float pressure, float voltage, float humidity)
    : id(id), temperature(temperature), pressure (pressure), voltage(voltage), humidity(humidity)
{}

Packet::Packet(const Packet& packet)
    : id(packet.id), temperature(packet.temperature), pressure (packet.pressure),
    voltage(packet.voltage), humidity(packet.humidity)
{}

void Packet::resetId() {
    nextId = 1;
}

int Packet::getNextId() {
    return nextId;
}

STRING_TYPE Packet::toString() {
#if IS_NOT_CONTROLLER
    auto result = std::stringstream();
    result << id << "|" << temperature << "|" << pressure << "|" << humidity << "|" << voltage;
    auto s = result.str();
#else
    auto s = String(id) + "|" + String(temperature) + "|" + String(pressure) + "|" + String(humidity) + "|" + String(voltage);
#endif
    // Generate and concatenate hash
    return s;
}

#endif