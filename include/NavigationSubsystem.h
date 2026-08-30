#ifndef NAVIGATIONSUBSYSTEM_H
#define NAVIGATIONSUBSYSTEM_H

class NavigationSubsystem {
private:
    double altitude;
    double velocity;

public: 
    NavigationSubsystem();

    void update();
    double getAltitude() const;
    double getVelocity() const;


};

#endif