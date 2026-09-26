#ifndef FAULTMANAGER_H
#define FAULTMANAGER_H

#include "SatelliteMode.h"

#include <cstdint>

enum FaultFlags : std::uint32_t {
    NO_FAULT = 0,
    LOW_BATTERY = 1 << 0,
    OVER_TEMPERATURE = 1 << 1
};

class FaultManager {
private:
    std::uint32_t activeFaults;

public:
    FaultManager();

    void update(
        double batteryLevel,
        double temperature
    );

    std::uint32_t getActiveFaults() const;

    SatelliteMode determineMode(
        SatelliteMode currentMode
    ) const;
};

#endif