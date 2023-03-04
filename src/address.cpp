#include "address.h"
#include "util.h"

#include <arpa/inet.h>
#include <cstring>
#include <memory>
#include <netdb.h>
#include <stdexcept>
#include <system_error>
#include <array>

using namespace std;
Address::Raw::operator sockaddr *() { return reinterpret_cast<sockaddr *>(&storage); }

Address::Raw::operator const sockaddr *() const { return reinterpret_cast<const sockaddr *>(&storage); }

Address::Address(const sockaddr *addr, const size_t size) : size_(size) {
    if (size > sizeof(address_.storage)) {
        throw runtime_error("invalid sockaddr size");
    }

    memcpy(&address_.storage, addr, size);
}


class gai_error_category : public error_category {
public:
    const char *name() const noexcept override { return "gai_error_category"; }
    string message(const int return_value) const noexcept override { return gai_strerror(return_value); }
};

Address::Address(const string &node, const string &service, const addrinfo &hints) : size_() {
    addrinfo *resolvedaddress_ = nullptr;
    const int gai_ret = getaddrinfo(node.c_str(), service.c_str(), &hints, &resolvedaddress_);
    if (gai_ret != 0) {
        throw tagged_error(gai_error_category(), "getaddrinfo(" + node + ", " + service + ")", gai_ret);
    }

    if (resolvedaddress_ == nullptr) {
        throw runtime_error("getaddrinfo returned successfully but with no results");
    }

    auto addrinfo_deleter = [](addrinfo *const x) { freeaddrinfo(x); };
    unique_ptr<addrinfo, decltype(addrinfo_deleter)> wrappedaddress_(resolvedaddress_, move(addrinfo_deleter));
    *this = Address(wrappedaddress_->ai_addr, wrappedaddress_->ai_addrlen);
}


static inline addrinfo make_hints(const int ai_flags, const int ai_family) {
    addrinfo hints{};
    hints.ai_flags = ai_flags;
    hints.ai_family = ai_family;
    return hints;
}

Address::Address(const string &hostname, const string &service)
    : Address(hostname, service, make_hints(AI_ALL, AF_INET)) {}

Address::Address(const string &ip, const uint16_t port)
    : Address(ip, ::to_string(port), make_hints(AI_NUMERICHOST | AI_NUMERICSERV, AF_INET)) {}

pair<string, uint16_t> Address::ip_port() const {
    array<char, NI_MAXHOST> ip{};
    array<char, NI_MAXSERV> port{};

    const int gni_ret =
        getnameinfo(address_, size_, ip.data(), ip.size(), port.data(), port.size(), NI_NUMERICHOST | NI_NUMERICSERV);
    if (gni_ret != 0) {
        throw tagged_error(gai_error_category(), "getnameinfo", gni_ret);
    }

    return {ip.data(), stoi(port.data())};
}

string Address::to_string() const {
    const auto ip_and_port = ip_port();
    return ip_and_port.first + ":" + ::to_string(ip_and_port.second);
}

uint32_t Address::ipv4_numeric() const {
    if (address_.storage.ss_family != AF_INET or size_ != sizeof(sockaddr_in)) {
        throw runtime_error("ipv4_numeric called on non-IPV4 address");
    }

    sockaddr_in ipv4_addr{};
    memcpy(&ipv4_addr, &address_.storage, size_);

    return ntohl(ipv4_addr.sin_addr.s_addr);
}

Address Address::from_ipv4_numeric(const uint32_t ipaddress_) {
    sockaddr_in ipv4_addr{};
    ipv4_addr.sin_family = AF_INET;
    ipv4_addr.sin_addr.s_addr = htonl(ipaddress_);

    return {reinterpret_cast<sockaddr *>(&ipv4_addr), sizeof(ipv4_addr)};
}

// equality
bool Address::operator==(const Address &other) const {
    if (size_ != other.size_) {
        return false;
    }

    return 0 == memcmp(&address_, &other.address_, size_);
}
