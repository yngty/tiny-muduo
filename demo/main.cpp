//
// Created by yngty on 2023/3/4.
//
#include "address.h"
#include "current_thread.h"
#include "event_loop.h"
#include "logger.h"
#include "socket.h"

int main(int argc, char *argv[]) {
  LOG_DEBUG("hello%d\n", CurrentThread::tid());
  Address address("www.baidu.com", "https");
  LOG_DEBUG(address.to_string().c_str());
  EventLoop loop{};
  loop.run_in_loop([] { LOG_DEBUG("hello%d\n", CurrentThread::tid()); });
  loop.loop();
  return 0;
}