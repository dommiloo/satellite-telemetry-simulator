#ifndef THERMALSUBSYSTEM_H
#define THERMALSUBSYSTEM_H

#include "OrbitEnvironment.h"

class ThermalSubsystem {
private:
    double temperature;

public:
    ThermalSubsystem();

    void update(OrbitEnvironment environment);
    double getTemperature() const;
};

#endif