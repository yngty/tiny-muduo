//
// Created by yngty on 2023/3/4.
//

#include "epoll_poller.h"
#include "channel.h"
#include "logger.h"
#include <cerrno>
#include <cstring>
#include <unistd.h>

const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;

EPollPoller::EPollPoller(EventLoop *loop)
    : Poller(loop), epollfd_(epoll_create1(EPOLL_CLOEXEC)),
      events_(kInitEventListSize) {
  if (epollfd_ < 0)
    LOG_FATAL("epoll_create error:%d \n", errno);
}

EPollPoller::~EPollPoller() { ::close(epollfd_); }

Timestamp EPollPoller::poll(int timeoutMs,
                            Poller::ChannelList *activeChannels) {
  LOG_INFO("func=%s => fd total count:%lu \n", __FUNCTION__, channels_.size());
  int numEvents = ::epoll_wait(epollfd_, &*events_.begin(),
                               static_cast<int>(events_.size()), timeoutMs);
  int saveErrno = errno;
  Timestamp now{Timestamp::now()};

  if (numEvents > 0) {
    LOG_INFO("%d events happened \n", numEvents);
    fill_active_channels(numEvents, activeChannels);
    if (numEvents == events_.size()) {
      events_.resize(events_.size() * 2);
    }
  } else if (numEvents == 0) {
    LOG_DEBUG("%s timeout! \n", __FUNCTION__);
  } else {
    if (saveErrno != EINTR) {
      errno = saveErrno;
      LOG_ERROR("EPollPoller::poll() err!");
    }
  }
  return now;
}

void EPollPoller::update_channel(Channel *channel) {
  const int index = channel->index();
  LOG_INFO("func=%s => fd=%d events=%d index=%d \n", __FUNCTION__,
           channel->fd(), channel->events(), index);

  if (index == kNew || index == kDeleted) {
    if (index == kNew) {
      int fd = channel->fd();
      channels_[fd] = channel;
    }

    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  } else {
    int fd = channel->fd();
    if (channel->is_none_event()) {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    } else {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

void EPollPoller::remove_Channel(Channel *channel) {
  int fd = channel->fd();
  channels_.erase(fd);

  LOG_INFO("func=%s => fd=%d\n", __FUNCTION__, fd);

  int index = channel->index();
  if (index == kAdded) {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}
void EPollPoller::fill_active_channels(int numEvents,
                                       Poller::ChannelList *activeChannels) {
  for (int i = 0; i < numEvents; ++i) {
    auto channel = static_cast<Channel *>(events_[i].data.ptr);
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}

void EPollPoller::update(int operation, Channel *channel) const {
  epoll_event event{};
  bzero(&event, sizeof event);

  int fd = channel->fd();
  event.events = channel->events();
  event.data.ptr = channel;
  event.data.fd = fd;

  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0) {
    if (operation == EPOLL_CTL_DEL) {
      LOG_ERROR("epoll_ctl del error:%d\n", errno);
    } else {
      LOG_FATAL("epoll_ctl add/mod error:%d\n", errno);
    }
  }
}
