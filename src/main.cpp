#include "Satellite.h"

#include <chrono>
#include <thread>

int main() {
    Satellite satellite("SAT-01");

    while (true) {
        satellite.update();
        satellite.printTelemetry();

        std::this_thread::sleep_for(
            std::chrono::seconds(1)
        );
    }

    return 0;
}