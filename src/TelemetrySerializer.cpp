#include "TelemetrySerializer.h"

#include <bit>
#include <stdexcept>

namespace {

void writeUint32(
    std::vector<std::uint8_t>& buffer,
    std::uint32_t value
) {
    buffer.push_back((value >> 24) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back(value & 0xFF);
}

void writeUint64(
    std::vector<std::uint8_t>& buffer,
    std::uint64_t value
) {
    for (int shift = 56; shift >= 0; shift -= 8) {
        buffer.push_back(
            (value >> shift) & 0xFF
        );
    }
}

void writeDouble(
    std::vector<std::uint8_t>& buffer,
    double value
) {
    std::uint64_t bits =
        std::bit_cast<std::uint64_t>(value);

    writeUint64(buffer, bits);
}

std::uint32_t readUint32(
    const std::vector<std::uint8_t>& data,
    std::size_t& offset
) {
    if (offset + 4 > data.size()) {
        throw std::runtime_error(
            "Invalid telemetry packet"
        );
    }

    std::uint32_t value = 0;

    for (int i = 0; i < 4; i++) {
        value =
            (value << 8) |
            data[offset++];
    }

    return value;
}

std::uint64_t readUint64(
    const std::vector<std::uint8_t>& data,
    std::size_t& offset
) {
    if (offset + 8 > data.size()) {
        throw std::runtime_error(
            "Invalid telemetry packet"
        );
    }

    std::uint64_t value = 0;

    for (int i = 0; i < 8; i++) {
        value =
            (value << 8) |
            data[offset++];
    }

    return value;
}

double readDouble(
    const std::vector<std::uint8_t>& data,
    std::size_t& offset
) {
    std::uint64_t bits =
        readUint64(data, offset);

    return std::bit_cast<double>(bits);
}

}

std::vector<std::uint8_t>
TelemetrySerializer::serialize(
    const TelemetryPacket& packet
) {
    std::vector<std::uint8_t> buffer;

    writeUint32(
        buffer,
        packet.satelliteId
    );

    writeUint32(
        buffer,
        packet.sequenceNumber
    );

    writeUint64(
        buffer,
        packet.timestamp
    );

    writeDouble(
        buffer,
        packet.batteryLevel
    );

    writeDouble(
        buffer,
        packet.temperature
    );

    writeDouble(
        buffer,
        packet.altitude
    );

    writeDouble(
        buffer,
        packet.velocity
    );

    buffer.push_back(
        packet.inSunlight ? 1 : 0
    );

    return buffer;
}

TelemetryPacket
TelemetrySerializer::deserialize(
    const std::vector<std::uint8_t>& data
) {
    std::size_t offset = 0;

    TelemetryPacket packet;

    packet.satelliteId =
        readUint32(data, offset);

    packet.sequenceNumber =
        readUint32(data, offset);

    packet.timestamp =
        readUint64(data, offset);

    packet.batteryLevel =
        readDouble(data, offset);

    packet.temperature =
        readDouble(data, offset);

    packet.altitude =
        readDouble(data, offset);

    packet.velocity =
        readDouble(data, offset);

    if (offset >= data.size()) {
        throw std::runtime_error(
            "Invalid telemetry packet"
        );
    }

    packet.inSunlight =
        data[offset] != 0;

    return packet;
}