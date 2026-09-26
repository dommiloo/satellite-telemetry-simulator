#ifndef SATELLITEMODE_H
#define SATELLITEMODE_H

#include <cstdint>

enum class SatelliteMode : std::uint8_t {
    NOMINAL = 0,
    SAFE_MODE = 1,
    RECOVERY = 2
};

#endif