//
// Created by yngty on 2023/3/4.
//

#ifndef TINY_MUDUO_CHANNEL_H
#define TINY_MUDUO_CHANNEL_H

#include "noncopyable.h"
#include "timestamp.h"
#include <functional>
#include <memory>

class EventLoop;
class Channel : Noncopyable {
public:
  using EventCallback = std::function<void()>;
  using ReadEventCallback = std::function<void(Timestamp)>;

  Channel(EventLoop *loop, int fd);

  void handle_event(Timestamp receiveTime);

  void set_read_callback(ReadEventCallback cb) { read_callback_ = cb; }
  void set_write_callback(EventCallback cb) { write_callback_ = cb; }
  void set_error_callback(EventCallback cb) { error_callback_ = cb; }
  void set_close_callback(EventCallback cb) { close_callback_ = cb; }

  void tie(const std::shared_ptr<void> &);

  int fd() const { return fd_; }
  int events() const { return events_; }
  void set_revents(uint32_t revt) { revents_ = revt; }

  void enable_reading() {
    events_ |= kReadEvent;
    update();
  }
  void disable_reading() {
    events_ &= ~kReadEvent;
    update();
  }
  void enable_writing() {
    events_ |= kWriteEvent;
    update();
  }
  void disable_writing() {
    events_ &= ~kWriteEvent;
    update();
  }
  void disable_all() {
    events_ = kNoneEvent;
    update();
  }

  bool is_none_event() const { return events_ == kNoneEvent; }
  bool is_writing() const { return events_ & kWriteEvent; }
  bool is_reading() const { return events_ & kReadEvent; }
  int index() const { return index_; }
  void set_index(int idx) { index_ = idx; }

  EventLoop *owner_loop() { return loop_; }
  void remove();

private:
  void update();
  void hand_event_with_guard(Timestamp timestamp);

private:
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

  EventLoop *loop_;
  const int fd_;
  int events_{};
  uint32_t revents_{};
  int index_{-1};

  std::weak_ptr<void> tie_;
  bool tied_{false};

  ReadEventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback close_callback_;
  EventCallback error_callback_;
};

#endif // TINY_MUDUO_CHANNEL_H
