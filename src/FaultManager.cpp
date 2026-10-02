#include "FaultManager.h"

FaultManager::FaultManager()
    : detectedFaults(NO_FAULT),
      injectedFaults(NO_FAULT) {
}

void FaultManager::update(
    double batteryLevel,
    double temperature
) {
    // LOW BATTERY

    if (batteryLevel < 15.0) {
        detectedFaults |= LOW_BATTERY;
    }
    else if (batteryLevel > 25.0) {
        detectedFaults &= ~LOW_BATTERY;
    }

    // OVER TEMPERATURE

    if (temperature > 70.0) {
        detectedFaults |= OVER_TEMPERATURE;
    }
    else if (temperature < 60.0) {
        detectedFaults &= ~OVER_TEMPERATURE;
    }
}

void FaultManager::injectFault(
    std::uint32_t fault
) {
    injectedFaults |= fault;
}

void FaultManager::clearInjectedFaults() {
    injectedFaults = NO_FAULT;
}

std::uint32_t FaultManager::getActiveFaults() const {
    return detectedFaults | injectedFaults;
}

SatelliteMode FaultManager::determineMode(
    SatelliteMode currentMode
) const {
    if (getActiveFaults() != NO_FAULT) {
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