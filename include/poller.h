//
// Created by yngty on 2023/3/4.
//

#ifndef TINY_MUDUO_POLLER_H
#define TINY_MUDUO_POLLER_H

#include "noncopyable.h"
#include "timestamp.h"

#include <unordered_map>
#include <vector>

class EventLoop;
class Channel;

class Poller : Noncopyable {
public:
  using ChannelList = std::vector<Channel *>;

  explicit Poller(EventLoop *loop);
  virtual ~Poller() = default;

  virtual Timestamp poll(int timeoutMs, ChannelList *activeChannels) = 0;
  virtual void update_channel(Channel *channel) = 0;
  virtual void remove_Channel(Channel *channel) = 0;

  bool has_channel(Channel *channel) const;
  static Poller *new_default_poller(EventLoop *loop);

protected:
  using ChannelMap = std::unordered_map<int, Channel *>;
  ChannelMap channels_;

private:
  EventLoop *owner_loop_;
};

#endif // TINY_MUDUO_POLLER_H
