#include "UdpSender.h"

#include <stdexcept>

UdpSender::UdpSender(
    const std::string& ip,
    std::uint16_t port
) {
    WSADATA wsaData;

    if (
        WSAStartup(
            MAKEWORD(2, 2),
            &wsaData
        ) != 0
    ) {
        throw std::runtime_error(
            "Failed to initialize Winsock"
        );
    }

    socketFd =
        socket(
            AF_INET,
            SOCK_DGRAM,
            IPPROTO_UDP
        );

    if (socketFd == INVALID_SOCKET) {
        WSACleanup();

        throw std::runtime_error(
            "Failed to create UDP socket"
        );
    }

    destination = {};

    destination.sin_family =
        AF_INET;

    destination.sin_port =
        htons(port);

    if (
        inet_pton(
            AF_INET,
            ip.c_str(),
            &destination.sin_addr
        ) != 1
    ) {
        closesocket(socketFd);
        WSACleanup();

        throw std::runtime_error(
            "Invalid destination IP address"
        );
    }
}

UdpSender::~UdpSender() {
    closesocket(socketFd);

    WSACleanup();
}

void UdpSender::send(
    const std::vector<std::uint8_t>& data
) {
    int result =
        sendto(
            socketFd,
            reinterpret_cast<const char*>(
                data.data()
            ),
            static_cast<int>(
                data.size()
            ),
            0,
            reinterpret_cast<sockaddr*>(
                &destination
            ),
            sizeof(destination)
        );

    if (result == SOCKET_ERROR) {
        throw std::runtime_error(
            "Failed to send UDP packet"
        );
    }
}