//
// Created by yngty on 2023/3/4.
//

#include "current_thread.h"

namespace CurrentThread {
__thread int t_cached_tid = 0;
void cache_tid() {
  if (t_cached_tid == 0) {
    t_cached_tid = static_cast<pid_t>(::syscall(SYS_gettid));
  }
}
} // namespace CurrentThread
