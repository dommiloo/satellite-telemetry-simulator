#include "PowerSubsystem.h"

PowerSubsystem::PowerSubsystem()
    : batteryLevel(100.0) {
}

void PowerSubsystem::update(OrbitEnvironment environment) {
    if (environment == OrbitEnvironment::Sunlight) {
        batteryLevel += 0.05;
    } else {
        batteryLevel -= 0.15;
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