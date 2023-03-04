//
// Created by yngty on 2023/3/4.
//

#ifndef TINY_MUDUO_CURRENT_THREAD_H
#define TINY_MUDUO_CURRENT_THREAD_H

#include <sys/syscall.h>
#include <unistd.h>

namespace CurrentThread {
extern __thread int t_cached_tid;
void cache_tid();
inline int tid() {
  if (__builtin_expect(t_cached_tid == 0, 0)) {
    cache_tid();
  }
  return t_cached_tid;
}
} // namespace CurrentThread
#endif // TINY_MUDUO_CURRENT_THREAD_H
