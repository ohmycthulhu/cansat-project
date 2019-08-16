#ifndef __PACKET__
#define __PACKET__
#include "packet.hpp"
#include "md5.hpp"

#if IS_NOT_CONTROLLER
#include <sstream>
#endif

int Packet::nextId = 1;

Packet::Packet(const float temperature, const float pressure, const float voltage,
                const float humidity, const float height, const float speed, const float time,
            const double latitude, const double longitude, STRING_TYPE gpsTime)
    : id(nextId++), temperature(temperature), pressure (pressure), voltage(voltage),
    humidity(humidity), height(height), speed(speed), time(time),
    latitude(latitude), longitude(longitude), gpsTime(gpsTime) {}

Packet::Packet(const int id, const float temperature, const float pressure, const float voltage,
                const float humidity, const float height, const float speed, const float time,
            const double latitude, const double longitude, STRING_TYPE gpsTime)
    : id(id), temperature(temperature), pressure (pressure), voltage(voltage),
    humidity(humidity), height(height), speed(speed), time(time),
    latitude(latitude), longitude(longitude), gpsTime(gpsTime) {}

Packet::Packet(const Packet& packet)
    : id(packet.id), temperature(packet.temperature), pressure (packet.pressure),
    voltage(packet.voltage), humidity(packet.humidity), height(packet.height), speed(packet.speed),
    time(packet.time), latitude(packet.latitude), longitude(packet.longitude), gpsTime(packet.gpsTime) {}

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
            << voltage << "|"
            << height << "|"
            << speed << "|"
            << time;
    auto s = result.str();
#else
    auto s = String(id) + "|"
            + String(temperature) + "|"
            + String(pressure) + "|"
            + String(humidity) + "|"
            + String(voltage) + "|"
            + String(height) + "|"
            + String(speed) + "|"
            + String(time);
#endif

    return s;
}

#endif