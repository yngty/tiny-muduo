//
// Created by yngty on 2023/3/4.
//

#include "poller.h"
#include "channel.h"

Poller::Poller(EventLoop *loop) : owner_loop_(loop) {}

bool Poller::has_channel(Channel *channel) const {
  auto it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}
