#include "FaultManager.h"
#include "Satellite.h"
#include "TelemetrySerializer.h"
#include "UdpSender.h"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    Satellite satellite(
        "SAT-01",
        1
    );

    UdpSender sender(
        "127.0.0.1",
        5000
    );

    // Stores the most recent command entered by the user.
    // 0 means there is currently no command waiting.
    std::atomic<int> pendingCommand{0};

    std::cout
        << "Fault Injection Controls:\n"
        << "1 = Inject LOW_BATTERY\n"
        << "2 = Inject OVER_TEMPERATURE\n"
        << "3 = Clear injected faults\n\n";

    // Separate thread waits for keyboard input.
    std::thread inputThread(
        [&pendingCommand]() {
            int command;

            while (std::cin >> command) {
                pendingCommand.store(command);
            }
        }
    );

    // Let the input thread continue running independently.
    inputThread.detach();

    while (true) {

        // Grab the pending command and reset it to 0.
        int command =
            pendingCommand.exchange(0);

        switch (command) {

            case 1:
                satellite.injectFault(
                    LOW_BATTERY
                );

                std::cout
                    << "\n[INJECTED] LOW_BATTERY\n";

                break;

            case 2:
                satellite.injectFault(
                    OVER_TEMPERATURE
                );

                std::cout
                    << "\n[INJECTED] OVER_TEMPERATURE\n";

                break;

            case 3:
                satellite.clearInjectedFaults();

                std::cout
                    << "\n[CLEARED] Injected faults\n";

                break;

            default:
                break;
        }

        satellite.update();

        TelemetryPacket packet =
            satellite.generateTelemetry();

        std::vector<std::uint8_t> bytes =
            TelemetrySerializer::serialize(
                packet
            );

        sender.send(bytes);

        std::cout
            << "Sent telemetry packet "
            << packet.sequenceNumber
            << " ("
            << bytes.size()
            << " bytes)\n";

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}