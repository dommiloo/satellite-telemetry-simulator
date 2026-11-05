#ifndef POWERSUBSYSTEM_H
#define POWERSUBSYSTEM_H

#include "OrbitEnvironment.h"
#include "SatelliteMode.h"

class PowerSubsystem {
private:
    double batteryLevel;

public:
    PowerSubsystem();

    void update(
        OrbitEnvironment environment,
        SatelliteMode mode
    );

    double getBatteryLevel() const;
};

#endif