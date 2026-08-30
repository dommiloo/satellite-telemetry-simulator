#include "ThermalSubsystem.h"

ThermalSubsystem::ThermalSubsystem()
    : temperature(20.0) {
}

void ThermalSubsystem::update() {
    temperature += 0.05;
}

double ThermalSubsystem::getTemperature() const {
    return temperature;
}