//
// Created by yanghao on 2023/3/4.
//

#include "socket.h"
#include "address.h"
Socket::Socket(const int domain, const int type)
    : Socket(socket(domain, type, 0), domain, type) {}

Socket::Socket(int fd, const int domain, const int type) : fd_(fd) {}

template <typename option_type>
void Socket::setsockopt(const int level, const int option,
                        const option_type &option_value) {
  ::setsockopt(fd_, level, option, &option_value, sizeof option_value);
}

void Socket::bind(const Address &address) {
  ::bind(fd_, address, address.size());
}

void Socket::connect(const Address &address) {
  ::connect(fd_, address, address.size());
}

void Socket::shutdown(const int how) { ::shutdown(fd_, how); }

Address Socket::get_address(
    const std::string &name_of_function,
    const std::function<int(int, sockaddr *, socklen_t *)> &function) const {
  Address::Raw address;
  socklen_t size = sizeof(address);
  function(fd_, address, &size);
  return {address, size};
}
Address Socket::local_address() const {
  return get_address("getsockname", getsockname);
}

Address Socket::peer_address() const {
  return get_address("getpeername", getpeername);
}

void Socket::set_reuseaddr() {
  setsockopt(SOL_SOCKET, SO_REUSEADDR, static_cast<int>(true));
}
