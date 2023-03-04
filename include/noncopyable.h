//
// Created by yanghao on 2023/3/4.
//

#ifndef TINY_MUDUO_INCLUDE_NONCOPYABLE_H
#define TINY_MUDUO_INCLUDE_NONCOPYABLE_H
class Noncopyable {
public:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
};
#endif //TINY_MUDUO_INCLUDE_NONCOPYABLE_H
