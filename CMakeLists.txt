cmake_minimum_required(VERSION 3.16)

project(SatelliteTelemetrySimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    satellite_simulator

    src/main.cpp
    src/Satellite.cpp
    src/PowerSubsystem.cpp
    src/ThermalSubsystem.cpp
    src/NavigationSubsystem.cpp
)

target_include_directories(
    satellite_simulator
    PRIVATE include
)