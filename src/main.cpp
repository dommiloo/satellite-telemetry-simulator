#include "Satellite.h"
#include "TelemetrySerializer.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    Satellite satellite("SAT-01", 1);

    while (true) {
        satellite.update();

        TelemetryPacket original =
            satellite.generateTelemetry();

       
        std::vector<std::uint8_t> bytes =
            TelemetrySerializer::serialize(original);

        TelemetryPacket reconstructed =
            TelemetrySerializer::deserialize(bytes);

        satellite.printTelemetry(reconstructed);

        std::cout << "Serialized size: "
                  << bytes.size()
                  << " bytes\n";

        std::cout << "=============================\n";

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}