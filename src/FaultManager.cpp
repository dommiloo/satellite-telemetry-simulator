#include "FaultManager.h"

FaultManager::FaultManager()
    : activeFaults(NO_FAULT) {
}

void FaultManager::update(
    double batteryLevel,
    double temperature
) {
    // LOW BATTERY

    if (batteryLevel < 15.0) {
        activeFaults |= LOW_BATTERY;
    }
    else if (batteryLevel > 25.0) {
        activeFaults &= ~LOW_BATTERY;
    }

    // OVER TEMPERATURE

    if (temperature > 70.0) {
        activeFaults |= OVER_TEMPERATURE;
    }
    else if (temperature < 60.0) {
        activeFaults &= ~OVER_TEMPERATURE;
    }
}

std::uint32_t FaultManager::getActiveFaults() const {
    return activeFaults;
}

SatelliteMode FaultManager::determineMode(
    SatelliteMode currentMode
) const {
    if (activeFaults != NO_FAULT) {
        return SatelliteMode::SAFE_MODE;
    }

    if (currentMode == SatelliteMode::SAFE_MODE) {
        return SatelliteMode::RECOVERY;
    }

    if (currentMode == SatelliteMode::RECOVERY) {
        return SatelliteMode::NOMINAL;
    }

    return SatelliteMode::NOMINAL;
}