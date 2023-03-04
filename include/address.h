//
// Created by yanghao on 2023/3/4.
//

#ifndef TINY_MUDUO_ADDRESS_H
#define TINY_MUDUO_ADDRESS_H

#include <sys/socket.h>
#include <string>
#include <netdb.h>

class Address {
public:
    class Raw {
    public:
        sockaddr_storage storage {};
        operator sockaddr*();
        operator const sockaddr*() const;
    };
public:
    Address(const std::string &hostname, const std::string &service);
    Address(const std::string &ip, const std::uint16_t port = 0);
    Address(const sockaddr *addr, const std::size_t size);

    bool operator==(const Address &other) const;
    bool operator!=(const Address &other) const { return not operator==(other); }

    std::pair<std::string, uint16_t> ip_port() const;
    std::string ip() const { return ip_port().first; }
    uint16_t port() const { return ip_port().second; }
    uint32_t ipv4_numeric() const;
    static Address from_ipv4_numeric(const uint32_t ip_address);
    std::string to_string() const;

private:
    Address(const std::string& node, const std::string& service, const addrinfo& hints);
private:
    socklen_t size_;
    Raw address_;
};

#endif //TINY_MUDUO_ADDRESS_H
