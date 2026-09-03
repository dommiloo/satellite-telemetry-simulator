#ifndef TELEMETRYPACKET_H
#define TELEMETRYPACKET_H

#include <cstdint>

struct TelemetryPacket {
    std::uint32_t satelliteId;
    std::uint32_t sequenceNumber;
    std::uint64_t timestamp;

    double batteryLevel;
    double temperature;
    double altitude;
    double velocity;

    bool inSunlight;
};

#endif