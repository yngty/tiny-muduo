//
// Created by yngty on 2023/3/4.
//

#ifndef TINY_MUDUO_TIMESTAMP_H
#define TINY_MUDUO_TIMESTAMP_H

#include <string>

class Timestamp {
public:
  Timestamp() : Timestamp(0){};
  explicit Timestamp(int64_t microSecondsSinceEpoch)
      : microSecondsSinceEpoch_(microSecondsSinceEpoch) {}
  static Timestamp now();
  std::string to_string() const;

private:
  int64_t microSecondsSinceEpoch_;
};

#endif // TINY_MUDUO_TIMESTAMP_H
