#include "TelemetrySerializer.h"

#include <bit>
#include <cstddef>
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
        std::bit_cast<std::uint64_t>(
            value
        );

    writeUint64(
        buffer,
        bits
    );
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
        readUint64(
            data,
            offset
        );

    return std::bit_cast<double>(
        bits
    );
}

std::uint32_t calculateCrc32(
    const std::vector<std::uint8_t>& data,
    std::size_t length
) {
    std::uint32_t crc =
        0xFFFFFFFF;

    for (
        std::size_t i = 0;
        i < length;
        i++
    ) {
        crc ^= data[i];

        for (
            int bit = 0;
            bit < 8;
            bit++
        ) {
            if (crc & 1) {
                crc =
                    (crc >> 1) ^
                    0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc ^ 0xFFFFFFFF;
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

    std::uint32_t checksum =
        calculateCrc32(
            buffer,
            buffer.size()
        );

    writeUint32(
        buffer,
        checksum
    );

    return buffer;
}

TelemetryPacket
TelemetrySerializer::deserialize(
    const std::vector<std::uint8_t>& data
) {
    constexpr std::size_t PAYLOAD_SIZE =
        49;

    constexpr std::size_t PACKET_SIZE =
        53;

    if (data.size() != PACKET_SIZE) {
        throw std::runtime_error(
            "Invalid telemetry packet size"
        );
    }

    std::size_t offset = 0;

    TelemetryPacket packet;

    packet.satelliteId =
        readUint32(
            data,
            offset
        );

    packet.sequenceNumber =
        readUint32(
            data,
            offset
        );

    packet.timestamp =
        readUint64(
            data,
            offset
        );

    packet.batteryLevel =
        readDouble(
            data,
            offset
        );

    packet.temperature =
        readDouble(
            data,
            offset
        );

    packet.altitude =
        readDouble(
            data,
            offset
        );

    packet.velocity =
        readDouble(
            data,
            offset
        );

    packet.inSunlight =
        data[offset++] != 0;

    packet.checksum =
        readUint32(
            data,
            offset
        );

    std::uint32_t calculatedChecksum =
        calculateCrc32(
            data,
            PAYLOAD_SIZE
        );

    if (
        packet.checksum !=
        calculatedChecksum
    ) {
        throw std::runtime_error(
            "CRC validation failed"
        );
    }

    return packet;
}