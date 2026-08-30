#include "PowerSubsystem.h"

PowerSubsystem::PowerSubsystem()
    : batteryLevel(100.0) {
}

void PowerSubsystem::update() {
    batteryLevel -= 0.1;
}

double PowerSubsystem::getBatteryLevel() const {
    return batteryLevel;
}