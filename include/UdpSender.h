#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <cstdint>
#include <string>
#include <vector>

class UdpSender {
private:
    std::string destinationIp;
    std::uint16_t destinationPort;

public:
    UdpSender(
        const std::string& ip,
        std::uint16_t port
    );

    void send(
        const std::vector<std::uint8_t>& data
    );
};

#endif