//
// Created by yngty on 2023/3/4.
//

#include "timestamp.h"
#include <ctime>

Timestamp Timestamp::now() { return Timestamp(time(nullptr)); }

std::string Timestamp::to_string() const {
  char buf[128] = {0};
  tm *tm_time = localtime(&microSecondsSinceEpoch_);
  snprintf(buf, 128, "%4d/%02d/%02d %02d:%02d:%02d", tm_time->tm_year + 1900,
           tm_time->tm_mon + 1, tm_time->tm_mday, tm_time->tm_hour,
           tm_time->tm_min, tm_time->tm_sec);
  return buf;
}