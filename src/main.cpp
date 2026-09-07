#include "Satellite.h"
#include "TelemetrySerializer.h"
#include "UdpSender.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    Satellite satellite("SAT-01", 1);

    UdpSender sender(
        "127.0.0.1",
        5000
    );

    while (true) {
        satellite.update();

        TelemetryPacket packet =
            satellite.generateTelemetry();

        std::vector<std::uint8_t> bytes =
            TelemetrySerializer::serialize(packet);

        sender.send(bytes);

        std::cout
            << "Sent telemetry packet "
            << packet.sequenceNumber
            << " ("
            << bytes.size()
            << " bytes)"
            << '\n';

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}