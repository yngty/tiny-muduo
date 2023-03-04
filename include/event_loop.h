//
// Created by yngty on 2023/3/4.
//

#ifndef TINY_MUDUO_EVENT_LOOP_H
#define TINY_MUDUO_EVENT_LOOP_H

#include "current_thread.h"
#include "noncopyable.h"
#include "timestamp.h"
#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

class Channel;
class Poller;

class EventLoop : Noncopyable {
public:
  using Functor = std::function<void()>;
  EventLoop();
  ~EventLoop();

  void loop();
  void quit();

  [[nodiscard]] Timestamp poll_return_time() const { return poll_return_time_; }
  void run_in_loop(Functor cb);
  void queue_in_loop(Functor cb);
  void wakeup() const;
  void update_channel(Channel *channel);
  void remove_channel(Channel *channel);
  bool has_channel(Channel *channel);

  [[nodiscard]] bool is_in_loop_thread() const {
    return thread_id_ == CurrentThread::tid();
  }

private:
  void handle_read() const;
  void do_pending_functors();

private:
  using ChannelList = std::vector<Channel *>;

  std::atomic_bool looping_{false};
  std::atomic_bool quit_{false};
  const pid_t thread_id_;

  Timestamp poll_return_time_;
  std::unique_ptr<Poller> poller_;

  int wake_fd_;
  std::unique_ptr<Channel> wakeup_channel_;

  ChannelList active_channels_;

  std::atomic_bool calling_pending_functors_{false};
  std::vector<Functor> pending_functors_;
  std::mutex mutex_;
};

#endif // TINY_MUDUO_EVENT_LOOP_H
