#include "Satellite.h"

#include <iostream>

Satellite::Satellite(const std::string& name)
    : name(name) {
}

void Satellite::update() {
    power.update();
    thermal.update();
    navigation.update();
}

void Satellite::printTelemetry() const {
    std::cout << "Satellite: " << name << '\n';

    std::cout << "Battery: "
              << power.getBatteryLevel()
              << "%\n";

    std::cout << "Temperature: "
              << thermal.getTemperature()
              << " C\n";

    std::cout << "Altitude: "
              << navigation.getAltitude()
              << " kml\n";

    std::cout << "Velocity: "
              << navigation.getVelocity()
              << " km/s\n";

    std::cout << "-----------------------------\n";
}