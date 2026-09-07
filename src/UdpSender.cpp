#include "UdpSender.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdexcept>

UdpSender::UdpSender(
    const std::string& ip,
    std::uint16_t port
)
    : destinationIp(ip),
      destinationPort(port) {
}

void UdpSender::send(
    const std::vector<std::uint8_t>& data
) {
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error(
            "Failed to initialize Winsock"
        );
    }

    SOCKET socketFd =
        socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (socketFd == INVALID_SOCKET) {
        WSACleanup();

        throw std::runtime_error(
            "Failed to create UDP socket"
        );
    }

    sockaddr_in destination{};

    destination.sin_family = AF_INET;
    destination.sin_port =
        htons(destinationPort);

    if (
        inet_pton(
            AF_INET,
            destinationIp.c_str(),
            &destination.sin_addr
        ) != 1
    ) {
        closesocket(socketFd);
        WSACleanup();

        throw std::runtime_error(
            "Invalid destination IP"
        );
    }

    int result =
        sendto(
            socketFd,
            reinterpret_cast<const char*>(
                data.data()
            ),
            static_cast<int>(data.size()),
            0,
            reinterpret_cast<sockaddr*>(
                &destination
            ),
            sizeof(destination)
        );

    if (result == SOCKET_ERROR) {
        closesocket(socketFd);
        WSACleanup();

        throw std::runtime_error(
            "Failed to send UDP packet"
        );
    }

    closesocket(socketFd);
    WSACleanup();
}