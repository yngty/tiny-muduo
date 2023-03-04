//
// Created by yngty on 2023/3/4.
//

#ifndef TINY_MUDUO_EPOLL_POLLER_H
#define TINY_MUDUO_EPOLL_POLLER_H

#include "poller.h"
#include <sys/epoll.h>

class EPollPoller : public Poller {
public:
  EPollPoller(EventLoop *loop);
  virtual ~EPollPoller() override;
  virtual Timestamp poll(int timeoutMs, std::vector<Channel *> *activeChannels);
  virtual void update_channel(Channel *channel);
  virtual void remove_Channel(Channel *channel);

private:
  static constexpr int kInitEventListSize = 16;

  void fill_active_channels(int numEvents, ChannelList *activeChannels);
  void update(int operation, Channel *channel) const;

  using EventList = std::vector<epoll_event>;

  int epollfd_;
  EventList events_;
};

#endif // TINY_MUDUO_EPOLL_POLLER_H
