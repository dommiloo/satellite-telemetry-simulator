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
    src/TelemetrySerializer.cpp
    src/UdpSender.cpp
)

target_include_directories(
    satellite_simulator
    PRIVATE include
)

target_link_libraries(
    satellite_simulator
    PRIVATE ws2_32
)

add_executable(
    ground_station
    ground_station/main.cpp
    src/TelemetrySerializer.cpp
)

target_include_directories(
    ground_station
    PRIVATE include
)

target_link_libraries(
    ground_station
    PRIVATE ws2_32
)