# Satellite Telemetry Simulator

A C++20 satellite telemetry and ground-station simulation focused on systems programming, networking, serialization, and fault-tolerant software design.

The simulator models several satellite subsystems, packages their state into a compact binary telemetry protocol, and transmits telemetry over UDP to a separate ground-station process for monitoring and analysis.

## Features

### Implemented

- Modular satellite architecture
- Power subsystem
- Thermal subsystem
- Navigation subsystem
- Simulated sunlight and eclipse states
- Telemetry packet generation
- Satellite ID and sequence numbering
- Millisecond timestamps
- Binary serialization and deserialization
- Custom binary telemetry packet format
- UDP telemetry transmission
- Separate ground-station executable
- Ground-station telemetry display
- Packet-loss detection using sequence numbers
- CRC32 telemetry integrity validation

### Planned

- Persistent UDP socket management
- Packet corruption simulation
- Fault injection
- Satellite operating modes
- SAFE_MODE transitions
- Command transmission from ground station to satellite
- Multithreaded subsystem execution
- Unit and integration testing
- GitHub Actions CI
- Improved telemetry logging
- Multiple satellite support
- Ground-station dashboard

## Architecture

```text
          +----------------------+
          |  Satellite Simulator |
          +----------+-----------+
                     |
                     v
          +----------------------+
          | Satellite Subsystems |
          |                      |
          |  Power               |
          |  Thermal             |
          |  Navigation          |
          +----------+-----------+
                     |
                     v
          +----------------------+
          |   TelemetryPacket    |
          +----------+-----------+
                     |
                     v
          +----------------------+
          | TelemetrySerializer  |
          +----------+-----------+
                     |
                     v
           Binary Telemetry Packet
                     |
                     v
                   UDP
                     |
                     v
          +----------------------+
          |    Ground Station    |
          +----------+-----------+
                     |
                     v
          +----------------------+
          | CRC Validation       |
          +----------+-----------+
                     |
                     v
          +----------------------+
          |   Deserialize Data   |
          +----------+-----------+
                     |
                     v
          +----------------------+
          | Telemetry Monitoring |
          +----------------------+