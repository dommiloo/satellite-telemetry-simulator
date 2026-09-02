#ifndef POWERSUBSYSTEM_H
#define POWERSUBSYSTEM_H

#include "OrbitEnvironment.h"

class PowerSubsystem {
private:
    double batteryLevel;

public:
    PowerSubsystem();

    void update(OrbitEnvironment environment);
    double getBatteryLevel() const;
};

#endif