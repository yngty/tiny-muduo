//
// Created by yngty on 2023/3/4.
//
#include "epoll_poller.h"

Poller *Poller::new_default_poller(EventLoop *loop) {
  if (::getenv("MUDUO_USE_POLL")) {
    return nullptr; // 生成poll的实例
  } else {
    return new EPollPoller(loop); // 生成epoll的实例
  }
}