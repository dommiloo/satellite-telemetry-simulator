#ifndef SATELLITE_H
#define SATELLITE_H

#include <string>

#include "PowerSubsystem.h"
#include "ThermalSubsystem.h"
#include "NavigationSubsystem.h"
#include "OrbitEnvironment.h"

class Satellite {
private:
    std::string name;

    PowerSubsystem power;
    ThermalSubsystem thermal;
    NavigationSubsystem navigation;

    OrbitEnvironment environment;
    int updateCount;

public:
    Satellite(const std::string& name);

    void update();
    void printTelemetry() const;
};

#endif