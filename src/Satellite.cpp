#include "Satellite.h"

#include <iostream>

Satellite::Satellite(const std::string& name)
    : name(name),
      environment(OrbitEnvironment::Sunlight),
      updateCount(0) {
}

void Satellite::update() {
    updateCount++;

    if (updateCount % 20 < 10) {
        environment = OrbitEnvironment::Sunlight;
    } else {
        environment = OrbitEnvironment::Eclipse;
    }

    power.update(environment);
    thermal.update(environment);
    navigation.update();
}

void Satellite::printTelemetry() const {
    std::cout << "Satellite: " << name << '\n';

    std::cout << "Environment: ";

    if (environment == OrbitEnvironment::Sunlight) {
        std::cout << "SUNLIGHT\n";
    } else {
        std::cout << "ECLIPSE\n";
    }

    std::cout << "Battery: "
              << power.getBatteryLevel()
              << "%\n";

    std::cout << "Temperature: "
              << thermal.getTemperature()
              << " C\n";

    std::cout << "Altitude: "
              << navigation.getAltitude()
              << " km\n";

    std::cout << "Velocity: "
              << navigation.getVelocity()
              << " km/s\n";

    std::cout << "-----------------------------\n";
}