#ifndef SATELLITE_H
#define SATELLITE_H

#include <string>
#include <cstdint>

#include "PowerSubsystem.h"
#include "ThermalSubsystem.h"
#include "NavigationSubsystem.h"
#include "OrbitEnvironment.h"
#include "TelemetryPacket.h"

class Satellite {
private:
    std::string name;

    PowerSubsystem power;
    ThermalSubsystem thermal;
    NavigationSubsystem navigation;

    OrbitEnvironment environment;

    std::uint32_t satelliteId;
    std::uint32_t sequenceNumber;

    int updateCount;

public:
    Satellite(const std::string& name, std::uint32_t satelliteId);

    void update();

    TelemetryPacket generateTelemetry();

    void printTelemetry(const TelemetryPacket& packet) const;
};

#endif