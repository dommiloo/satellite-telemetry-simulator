#ifndef SATELLITE_H
#define SATELLITE_H

#include <string>

class Satellite {
private:
    std::string name;
    double batteryLevel;
    double temperature;
    double altitude;
    double velocity;

public:
    Satellite(const std::string& name);

    void update();
    void printTelemetry() const;
};

#endif