# Satellite Telemetry Simulator

A C++20 systems project that simulates satellite telemetry generation, binary packet transmission, fault detection, and ground-station monitoring.

The project models several satellite subsystems, packages their state into a custom binary telemetry protocol, and sends telemetry over UDP to a separate ground-station process. The ground station validates packet integrity, detects packet loss, and monitors satellite operating state and faults.

## Overview

The simulator is split into two independent processes:

```text
Satellite Simulator
        |
        | UDP
        v
Ground Station
```

The satellite generates telemetry from simulated subsystems, serializes the data into a binary packet, calculates a CRC32 checksum, and sends the packet over UDP.

The ground station receives the packet, validates its CRC, deserializes the data, detects missing sequence numbers, and displays the satellite's current state.

## Architecture

```text
             SATELLITE SIMULATOR
                     |
        +------------+------------+
        |            |            |
      Power       Thermal     Navigation
        |            |            |
        +------------+------------+
                     |
                     v
                FaultManager
                     |
                     v
          NOMINAL / SAFE_MODE /
                RECOVERY
                     |
                     v
             TelemetryPacket
                     |
                     v
          Binary Serialization
                     |
                     v
                  CRC32
                     |
                     v
                   UDP
                     |
                     v
              GROUND STATION
                     |
                     v
              CRC Validation
                     |
                     v
            Deserialization
                     |
                     v
         Packet / Fault Monitoring
```

## Core Features

- Modular power, thermal, and navigation subsystems
- Simulated sunlight and eclipse environments
- Custom binary telemetry protocol
- Manual serialization and deserialization
- UDP communication using Winsock
- Separate satellite and ground-station executables
- Sequence-number based packet-loss detection
- CRC32 packet-integrity validation
- Automatic low-battery and over-temperature fault detection
- Manual fault injection
- Multiple simultaneous fault flags
- `NOMINAL`, `SAFE_MODE`, and `RECOVERY` operating states
- Automatic payload shutdown during non-nominal operation
- Reduced power consumption during `SAFE_MODE`
- Persistent UDP socket using RAII-style resource management

## Satellite Subsystems

### Power

The power subsystem tracks battery level and reacts to the simulated orbital environment.

```text
SUNLIGHT
→ battery charges

ECLIPSE
→ battery drains
```

During `SAFE_MODE`, the simulator reduces power consumption to model basic fault-response behavior.

Battery level is constrained between `0%` and `100%`.

### Thermal

The thermal subsystem tracks simulated spacecraft temperature.

Temperature rises during sunlight exposure and falls during eclipse.

The fault manager monitors this value and can trigger an `OVER_TEMPERATURE` fault when the configured threshold is exceeded.

### Navigation

The navigation subsystem currently tracks:

- Altitude
- Velocity

The current values are intentionally simplified because the focus of the project is software architecture, networking, and reliability rather than high-fidelity orbital mechanics.

## Telemetry Protocol

Every simulation cycle produces a `TelemetryPacket` containing the current satellite state.

Current fields include:

| Field | Purpose |
|---|---|
| Satellite ID | Identifies the spacecraft |
| Sequence Number | Detects missing packets |
| Timestamp | Records packet generation time |
| Battery Level | Power subsystem state |
| Temperature | Thermal subsystem state |
| Altitude | Navigation data |
| Velocity | Navigation data |
| Environment | Sunlight or eclipse |
| Operating Mode | NOMINAL, SAFE_MODE, or RECOVERY |
| Fault Flags | Active satellite faults |
| Payload State | Enabled or disabled |
| CRC32 | Packet integrity validation |

The packet is manually serialized into a deterministic binary representation before transmission.

## Binary Serialization

Rather than sending the in-memory C++ structure directly, each telemetry field is explicitly converted into bytes.

```text
TelemetryPacket
      |
      v
Manual Serialization
      |
      v
Binary Byte Buffer
```

The ground station reverses the process:

```text
Binary Byte Buffer
      |
      v
Deserialization
      |
      v
TelemetryPacket
```

Integer and floating-point fields are serialized individually, avoiding reliance on compiler-specific struct padding.

## UDP Communication

The satellite simulator and ground station run as separate processes.

For local development:

```text
IP:   127.0.0.1
Port: 5000
```

The satellite sends each telemetry packet over UDP:

```text
Satellite Simulator
        |
        | Serialized Telemetry
        | UDP
        v
Ground Station
```

UDP was selected because telemetry is continuously generated and newer state information is generally more useful than retransmitting old data.

## Packet-Loss Detection

Every telemetry packet contains a sequence number.

For example:

```text
Sequence 41
Sequence 42
Sequence 44
```

The ground station knows that:

```text
Sequence 43
```

was not received.

It reports:

```text
[WARNING] Missing 1 telemetry packet(s)
Expected sequence: 43
Received sequence: 44
```

## CRC32 Integrity Validation

Before transmission, the simulator calculates a CRC32 value over the serialized telemetry payload.

```text
Telemetry Data
      |
      v
Calculate CRC32
      |
      v
Payload + CRC
```

The ground station recalculates the CRC after receiving the packet.

```text
Received Payload
      |
      v
Calculate CRC
      |
      v
Compare with transmitted CRC
```

If the values match:

```text
CRC Status: VALID
```

If they do not match, the packet is rejected:

```text
[ERROR] Telemetry packet rejected:
CRC validation failed
```

CRC32 is used for accidental corruption detection and is not intended as encryption or authentication.

## Fault Management

The `FaultManager` monitors subsystem state and tracks active faults.

Current automatically detected faults include:

```text
LOW_BATTERY
OVER_TEMPERATURE
```

Multiple faults are represented using bit flags, allowing more than one condition to be active simultaneously.

For example:

```text
LOW_BATTERY
OVER_TEMPERATURE
```

can both be reported in the same telemetry packet.

## Manual Fault Injection

The simulator also supports manually injecting faults while it is running.

Controls:

```text
1 → Inject LOW_BATTERY
2 → Inject OVER_TEMPERATURE
3 → Clear injected faults
```

Keyboard input is handled by a separate thread so the simulation and telemetry transmission continue running while the program waits for commands.

Example:

```text
[INJECTED] LOW_BATTERY
```

The next ground-station update may show:

```text
Mode: SAFE_MODE
Faults: LOW_BATTERY
Payload: DISABLED
```

This allows failure behavior to be tested without manually changing subsystem values.

## Operating Modes

The satellite supports three operating modes:

```text
NOMINAL
SAFE_MODE
RECOVERY
```

### NOMINAL

Normal satellite operation.

```text
Payload: ENABLED
Normal power behavior
```

### SAFE_MODE

Triggered while one or more faults are active.

```text
Payload: DISABLED
Reduced power consumption
Fault remains active
```

### RECOVERY

Entered after faults clear.

```text
Payload: DISABLED
```

After a successful recovery cycle, the satellite returns to:

```text
NOMINAL
```

and the payload is re-enabled.

Typical transition:

```text
NOMINAL
   |
   | fault detected
   v
SAFE_MODE
   |
   | fault cleared
   v
RECOVERY
   |
   v
NOMINAL
```

## Ground Station Output

Example normal telemetry:

```text
-----------------------------
Satellite ID: 1
Sequence: 37
Timestamp: 1793923245000 ms
CRC Status: VALID
Mode: NOMINAL
Faults: NONE
Payload: ENABLED
Environment: SUNLIGHT
Battery: 99.8%
Temperature: 21.4 C
Altitude: 550 km
Velocity: 7.6 km/s
```

Example fault state:

```text
-----------------------------
Satellite ID: 1
Sequence: 38
CRC Status: VALID
Mode: SAFE_MODE
Faults: LOW_BATTERY
Payload: DISABLED
Environment: ECLIPSE
Battery: 99.7%
Temperature: 21.3 C
Altitude: 550 km
Velocity: 7.6 km/s
```

The battery can remain high during this example because `LOW_BATTERY` may have been manually injected for testing.

## Project Structure

```text
satellite-telemetry-simulator/
|
+-- include/
|   +-- Satellite.h
|   +-- SatelliteMode.h
|   +-- PowerSubsystem.h
|   +-- ThermalSubsystem.h
|   +-- NavigationSubsystem.h
|   +-- OrbitEnvironment.h
|   +-- FaultManager.h
|   +-- TelemetryPacket.h
|   +-- TelemetrySerializer.h
|   +-- UdpSender.h
|
+-- src/
|   +-- main.cpp
|   +-- Satellite.cpp
|   +-- PowerSubsystem.cpp
|   +-- ThermalSubsystem.cpp
|   +-- NavigationSubsystem.cpp
|   +-- FaultManager.cpp
|   +-- TelemetrySerializer.cpp
|   +-- UdpSender.cpp
|
+-- ground_station/
|   +-- main.cpp
|
+-- tests/
|
+-- CMakeLists.txt
+-- README.md
+-- .gitignore
```

## Building

### Requirements

- C++20-compatible compiler
- CMake 3.16+
- Windows
- Winsock2

Clone the repository:

```bash
git clone <repository-url>
cd satellite-telemetry-simulator
```

Create a build directory:

```bash
mkdir build
cd build
```

Configure CMake:

```bash
cmake ..
```

Build:

```bash
cmake --build . --config Debug
```

The build produces two executables:

```text
satellite_simulator.exe
ground_station.exe
```

## Running

Start the ground station first:

```powershell
.\Debug\ground_station.exe
```

Expected:

```text
Ground station listening on port 5000...
```

Open another terminal and start the satellite:

```powershell
.\Debug\satellite_simulator.exe
```

Expected satellite output:

```text
Sent telemetry packet 0 (59 bytes)
Sent telemetry packet 1 (59 bytes)
Sent telemetry packet 2 (59 bytes)
```

The ground station will begin displaying decoded telemetry.

## Technologies

- C++20
- CMake
- Winsock2
- UDP
- Binary serialization
- CRC32
- C++ threads
- Atomics
- Bit flags
- Git / GitHub

## Engineering Concepts Demonstrated

This project was built to practice systems-oriented software engineering concepts including:

- Modular system architecture
- Networking
- UDP sockets
- Binary protocols
- Serialization / deserialization
- Data integrity validation
- Fault detection
- Fault injection
- State machines
- Bit manipulation
- Resource management
- RAII
- Multithreading
- Atomic synchronization
- Failure recovery

## Project Status

The core simulator is feature-complete for the initial release.

Current functionality includes end-to-end telemetry generation and UDP transmission, a separate ground station, packet-loss detection, CRC32 validation, automatic and injected faults, operating-mode transitions, safe-mode behavior, and payload shutdown.

Remaining engineering work is focused on validation and repository polish rather than additional simulator functionality.

## Future Work

Potential future extensions include:

- Multiple satellite support
- Ground-to-satellite commands
- More realistic orbital simulation
- Additional fault types
- Persistent telemetry logging
- Graphical ground-station dashboard

These features are intentionally outside the scope of the initial release.