#include "TelemetrySerializer.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    WSADATA wsaData;

    if (
        WSAStartup(
            MAKEWORD(2, 2),
            &wsaData
        ) != 0
    ) {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }

    SOCKET socketFd =
        socket(
            AF_INET,
            SOCK_DGRAM,
            IPPROTO_UDP
        );

    if (socketFd == INVALID_SOCKET) {
        std::cerr << "Failed to create UDP socket\n";

        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(5000);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (
        bind(
            socketFd,
            reinterpret_cast<sockaddr*>(
                &serverAddress
            ),
            sizeof(serverAddress)
        ) == SOCKET_ERROR
    ) {
        std::cerr << "Failed to bind UDP socket\n";

        closesocket(socketFd);
        WSACleanup();

        return 1;
    }

    std::cout
        << "Ground station listening on port 5000...\n";

    std::uint32_t lastSequence = 0;
    bool firstPacket = true;

    while (true) {
        std::vector<std::uint8_t> buffer(1024);

        sockaddr_in senderAddress{};
        int senderAddressSize =
            sizeof(senderAddress);

        int bytesReceived =
            recvfrom(
                socketFd,
                reinterpret_cast<char*>(
                    buffer.data()
                ),
                static_cast<int>(
                    buffer.size()
                ),
                0,
                reinterpret_cast<sockaddr*>(
                    &senderAddress
                ),
                &senderAddressSize
            );

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr
                << "Failed to receive UDP packet\n";

            continue;
        }

        buffer.resize(bytesReceived);

        try {
            TelemetryPacket packet =
                TelemetrySerializer::deserialize(
                    buffer
                );

            // Check for missing sequence numbers
            if (!firstPacket) {
                std::uint32_t expectedSequence =
                    lastSequence + 1;

                if (
                    packet.sequenceNumber >
                    expectedSequence
                ) {
                    std::uint32_t missingPackets =
                        packet.sequenceNumber -
                        expectedSequence;

                    std::cout
                        << "\n[WARNING] Missing "
                        << missingPackets
                        << " telemetry packet(s)\n";

                    std::cout
                        << "Expected sequence: "
                        << expectedSequence
                        << '\n';

                    std::cout
                        << "Received sequence: "
                        << packet.sequenceNumber
                        << '\n';
                }
            }

            lastSequence =
                packet.sequenceNumber;

            firstPacket = false;

            std::cout
                << "\n-----------------------------\n";

            std::cout
                << "Satellite ID: "
                << packet.satelliteId
                << '\n';

            std::cout
                << "Sequence: "
                << packet.sequenceNumber
                << '\n';

            std::cout
                << "Timestamp: "
                << packet.timestamp
                << " ms\n";

            std::cout
                << "Environment: "
                << (
                    packet.inSunlight
                        ? "SUNLIGHT"
                        : "ECLIPSE"
                )
                << '\n';

            std::cout
                << "Battery: "
                << packet.batteryLevel
                << "%\n";

            std::cout
                << "Temperature: "
                << packet.temperature
                << " C\n";

            std::cout
                << "Altitude: "
                << packet.altitude
                << " km\n";

            std::cout
                << "Velocity: "
                << packet.velocity
                << " km/s\n";
        }
        catch (
            const std::exception& error
        ) {
            std::cerr
                << "Invalid telemetry packet: "
                << error.what()
                << '\n';
        }
    }

    closesocket(socketFd);
    WSACleanup();

    return 0;
}