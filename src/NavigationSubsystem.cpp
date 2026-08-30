#include "NavigationSubsystem.h"

NavigationSubsystem::NavigationSubsystem()
    : altitude(550.0),
      velocity(7.6) {
}

void NavigationSubsystem::update() {
}

double NavigationSubsystem::getAltitude() const {
    return altitude;
}

double NavigationSubsystem::getVelocity() const {
    return velocity;
}