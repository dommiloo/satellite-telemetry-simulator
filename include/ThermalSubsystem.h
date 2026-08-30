#ifndef THERMALSUBSYSTEM_H
#define THERMALSUBSYSTEM_H

class ThermalSubsystem {
private:
    double temperature;

public:
    ThermalSubsystem();

    void update();
    double getTemperature() const;
};

#endif