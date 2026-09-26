#ifndef SATELLITE_H
#define SATELLITE_H

#include <cstdint>
#include <string>

#include "FaultManager.h"
#include "NavigationSubsystem.h"
#include "OrbitEnvironment.h"
#include "PowerSubsystem.h"
#include "SatelliteMode.h"
#include "TelemetryPacket.h"
#include "ThermalSubsystem.h"

class Satellite {
private:
    std::string name;

    PowerSubsystem power;
    ThermalSubsystem thermal;
    NavigationSubsystem navigation;

    FaultManager faultManager;

    OrbitEnvironment environment;
    SatelliteMode mode;

    std::uint32_t satelliteId;
    std::uint32_t sequenceNumber;

    int updateCount;

public:
    Satellite(
        const std::string& name,
        std::uint32_t satelliteId
    );

    void update();

    TelemetryPacket generateTelemetry();

    void printTelemetry(
        const TelemetryPacket& packet
    ) const;
};

#endif