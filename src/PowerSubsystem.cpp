#include "PowerSubsystem.h"

PowerSubsystem::PowerSubsystem()
    : batteryLevel(100.0) {
}

void PowerSubsystem::update(
    OrbitEnvironment environment,
    SatelliteMode mode
) {
    double drainRate = 0.15;

    // SAFE_MODE reduces power consumption
    if (mode == SatelliteMode::SAFE_MODE) {
        drainRate = 0.05;
    }

    if (environment == OrbitEnvironment::Sunlight) {
        batteryLevel += 0.05;
    }
    else {
        batteryLevel -= drainRate;
    }

    if (batteryLevel > 100.0) {
        batteryLevel = 100.0;
    }

    if (batteryLevel < 0.0) {
        batteryLevel = 0.0;
    }
}

double PowerSubsystem::getBatteryLevel() const {
    return batteryLevel;
}