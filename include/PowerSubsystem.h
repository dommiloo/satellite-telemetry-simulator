#ifndef POWERSUBSYSTEM_H
#define POWERSUBSYSTEM_H

class PowerSubsystem {
private:
    double batteryLevel;

public:
    PowerSubsystem();

    void update();
    double getBatteryLevel() const;
};

#endif