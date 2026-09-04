#ifndef TELEMETRYSERIALIZER_H
#define TELEMETRYSERIALIZER_H

#include "TelemetryPacket.h"

#include <cstdint>
#include <vector>

class TelemetrySerializer {
public:
    static std::vector<std::uint8_t>
    serialize(const TelemetryPacket& packet);

    static TelemetryPacket
    deserialize(const std::vector<std::uint8_t>& data);
};

#endif