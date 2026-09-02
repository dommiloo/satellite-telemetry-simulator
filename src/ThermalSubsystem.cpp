#include "ThermalSubsystem.h"

ThermalSubsystem::ThermalSubsystem()
    : temperature(20.0) {
}

void ThermalSubsystem::update(OrbitEnvironment environment) {
    if (environment == OrbitEnvironment::Sunlight) {
        temperature += 0.10;
    } else {
        temperature -= 0.08;
    }
}

double ThermalSubsystem::getTemperature() const {
    return temperature;
}