#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <winsock2.h>
#include <ws2tcpip.h>

#include <cstdint>
#include <string>
#include <vector>

class UdpSender {
private:
    SOCKET socketFd;
    sockaddr_in destination;

public:
    UdpSender(
        const std::string& ip,
        std::uint16_t port
    );

    ~UdpSender();

    void send(
        const std::vector<std::uint8_t>& data
    );

    // Prevent accidental socket copies
    UdpSender(
        const UdpSender&
    ) = delete;

    UdpSender& operator=(
        const UdpSender&
    ) = delete;
};

#endif