#include "Satellite.h"

#include <iostream>

Satellite::Satellite(const std::string& name)
    : name(name),
      batteryLevel(100.0),
      temperature(20.0),
      altitude(550.0),
      velocity(7.6) {
}

void Satellite::update() {
    batteryLevel -= 0.1;
    temperature += 0.05;
}

void Satellite::printTelemetry() const {
    std::cout << "Satellite: " << name << '\n';
    std::cout << "Battery: " << batteryLevel << "%\n";
    std::cout << "Temperature: " << temperature << " C\n";
    std::cout << "Altitude: " << altitude << " km\n";
    std::cout << "Velocity: " << velocity << " km/s\n";
    std::cout << "-----------------------------\n";
}