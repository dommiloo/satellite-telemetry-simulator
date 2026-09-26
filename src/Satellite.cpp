#include "Satellite.h"

#include <chrono>
#include <iostream>

Satellite::Satellite(
    const std::string& name,
    std::uint32_t satelliteId
)
    : name(name),
      environment(OrbitEnvironment::Sunlight),
      mode(SatelliteMode::NOMINAL),
      satelliteId(satelliteId),
      sequenceNumber(0),
      updateCount(0) {
}

void Satellite::update() {
    updateCount++;

    if (updateCount % 20 < 10) {
        environment =
            OrbitEnvironment::Sunlight;
    }
    else {
        environment =
            OrbitEnvironment::Eclipse;
    }

    power.update(environment);
    thermal.update(environment);
    navigation.update();

    faultManager.update(
        power.getBatteryLevel(),
        thermal.getTemperature()
    );

    mode =
        faultManager.determineMode(mode);
}

TelemetryPacket Satellite::generateTelemetry() {
    TelemetryPacket packet;

    packet.satelliteId =
        satelliteId;

    packet.sequenceNumber =
        sequenceNumber++;

    packet.timestamp =
        std::chrono::duration_cast<
            std::chrono::milliseconds
        >(
            std::chrono::system_clock::now()
                .time_since_epoch()
        ).count();

    packet.batteryLevel =
        power.getBatteryLevel();

    packet.temperature =
        thermal.getTemperature();

    packet.altitude =
        navigation.getAltitude();

    packet.velocity =
        navigation.getVelocity();

    packet.inSunlight =
        environment ==
        OrbitEnvironment::Sunlight;

    packet.mode =
        mode;

    packet.faultFlags =
        faultManager.getActiveFaults();

    packet.checksum = 0;

    return packet;
}

void Satellite::printTelemetry(
    const TelemetryPacket& packet
) const {
    std::cout
        << "Satellite: "
        << name
        << '\n';

    std::cout
        << "Satellite ID: "
        << packet.satelliteId
        << '\n';

    std::cout
        << "Sequence: "
        << packet.sequenceNumber
        << '\n';

    std::cout
        << "Timestamp: "
        << packet.timestamp
        << " ms\n";

    std::cout
        << "Environment: "
        << (
            packet.inSunlight
                ? "SUNLIGHT"
                : "ECLIPSE"
        )
        << '\n';

    std::cout
        << "Battery: "
        << packet.batteryLevel
        << "%\n";

    std::cout
        << "Temperature: "
        << packet.temperature
        << " C\n";

    std::cout
        << "Altitude: "
        << packet.altitude
        << " km\n";

    std::cout
        << "Velocity: "
        << packet.velocity
        << " km/s\n";

    std::cout
        << "-----------------------------\n";
}