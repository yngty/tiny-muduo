//
// Created by yngty on 2023/3/4.
//

#include "event_loop.h"
#include "channel.h"
#include "logger.h"
#include "poller.h"

#include <sys/eventfd.h>

__thread EventLoop *t_loop_in_this_thread = nullptr;
const int kPollTimeMs = 10000;

static int create_event_fd() {
  int event_fd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (event_fd < 0) {
    LOG_FATAL("eventfd error:%d \n", errno);
  }
  return event_fd;
}
EventLoop::EventLoop()
    : looping_(false), quit_(false), calling_pending_functors_(false),
      thread_id_(CurrentThread::tid()),
      poller_(Poller::new_default_poller(this)), wake_fd_(create_event_fd()),
      wakeup_channel_(new Channel(this, wake_fd_)) {
  LOG_INFO("EventLoop created %p in thread %d \n", this, thread_id_);
  if (t_loop_in_this_thread) {
    LOG_FATAL("Another EventLoop %p exists in this thread %d \n",
              t_loop_in_this_thread, thread_id_);
  } else {
    t_loop_in_this_thread = this;
  }

  wakeup_channel_->set_read_callback(std::bind(&EventLoop::handle_read, this));
  wakeup_channel_->enable_reading();
}

EventLoop::~EventLoop() {
  wakeup_channel_->disable_all();
  wakeup_channel_->remove();
  ::close(wake_fd_);
  t_loop_in_this_thread = nullptr;
}

void EventLoop::loop() {
  looping_ = true;
  quit_ = false;
  LOG_INFO("EventLoop %p start looping \n", this);

  while (!quit_) {
    active_channels_.clear();
    poll_return_time_ = poller_->poll(kPollTimeMs, &active_channels_);
    for (Channel *channel : active_channels_) {
      channel->handle_event(poll_return_time_);
    }

    do_pending_functors();
  }
  LOG_INFO("EventLoop %p stop looping. \n", this);
  looping_ = false;
}

void EventLoop::quit() {
  quit_ = true;
  if (!is_in_loop_thread())
    wakeup();
}

void EventLoop::run_in_loop(EventLoop::Functor cb) {
  if (is_in_loop_thread()) {
    cb();
  } else {
    queue_in_loop(cb);
  }
}

void EventLoop::queue_in_loop(EventLoop::Functor cb) {
  {
    std::unique_lock lk(mutex_);
    pending_functors_.emplace_back(std::move(cb));
  }

  if (!is_in_loop_thread() || calling_pending_functors_)
    wakeup();
}

void EventLoop::wakeup() const {
  uint64_t one = 1;
  ssize_t n = write(wake_fd_, &one, sizeof one);
  if (n != sizeof one) {
    LOG_ERROR("EventLoop::wakeup() writes %lu bytes instead of 8 \n", n);
  }
}

void EventLoop::update_channel(Channel *channel) {
  return poller_->update_channel(channel);
}

void EventLoop::remove_channel(Channel *channel) {
  return poller_->remove_Channel(channel);
}

bool EventLoop::has_channel(Channel *channel) {
  return poller_->has_channel(channel);
}

void EventLoop::handle_read() const {
  uint64_t one;
  ssize_t n = read(wake_fd_, &one, sizeof one);
  if (n != sizeof one)
    LOG_ERROR("EventLoop::handleRead() reads %lu bytes instead of 8", n);
}

void EventLoop::do_pending_functors() {
  std::vector<Functor> functors;
  calling_pending_functors_ = true;
  {
    std::unique_lock lk(mutex_);
    functors.swap(pending_functors_);
  }

  for (const Functor &functor : functors) {
    functor();
  }
  calling_pending_functors_ = false;
}
