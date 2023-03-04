//
// Created by yanghao on 2023/3/4.
//

#ifndef TINY_MUDUO_SOCKET_H
#define TINY_MUDUO_SOCKET_H

#include "noncopyable.h"
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>

class Address;
class Socket : Noncopyable {
protected:
  Socket(const int domain, const int type);
  Socket(int fd, const int domain, const int type);

  template <typename option_type>
  void setsockopt(const int level, const int option,
                  const option_type &optionValue);

public:
  void bind(const Address &address);
  void connect(const Address &address);
  void shutdown(const int how);

  Address local_address() const;
  Address peer_address() const;

  void set_reuseaddr();

private:
  Address get_address(
      const std::string &name_of_function,
      const std::function<int(int, sockaddr *, socklen_t *)> &function) const;

private:
  int fd_;
};

#endif // TINY_MUDUO_SOCKET_H
