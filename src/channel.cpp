//
// Created by yngty on 2023/3/4.
//

#include "channel.h"
#include "event_loop.h"
#include "logger.h"
#include <sys/epoll.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd) {}

void Channel::handle_event(Timestamp receiveTime) {
  if (tied_) {
    std::shared_ptr<void> guard = tie_.lock();
    if (guard) {
      hand_event_with_guard(receiveTime);
    }
  } else {
    hand_event_with_guard(receiveTime);
  }
}

void Channel::tie(const std::shared_ptr<void> &obj) {
  tie_ = obj;
  tied_ = true;
}

void Channel::remove() { loop_->remove_channel(this); }

void Channel::update() { loop_->update_channel(this); }

void Channel::hand_event_with_guard(Timestamp timestamp) {
  LOG_INFO("channel handleEvent revents:%d\n", revents_);

  if ((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
    if (close_callback_)
      close_callback_();
  }

  if (revents_ & EPOLLERR) {
    if (error_callback_)
      error_callback_();
  }

  if (revents_ & (EPOLLIN | EPOLLPRI)) {
    if (read_callback_)
      read_callback_(timestamp);
  }

  if (revents_ & EPOLLOUT) {
    if (write_callback_)
      write_callback_();
  }
}
