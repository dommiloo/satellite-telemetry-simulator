#include "Satellite.h"

#include <chrono>
#include <thread>

int main() {
    Satellite satellite("SAT-01", 1);

    while (true) {
        satellite.update();

        TelemetryPacket packet =
            satellite.generateTelemetry();

        satellite.printTelemetry(packet);

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}