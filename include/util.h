//
// Created by yanghao on 2023/3/4.
//

#ifndef TINY_MUDUO_UTIL_H
#define TINY_MUDUO_UTIL_H
#include <system_error>

class tagged_error : public std::system_error {
public:
    tagged_error(const std::error_category& category, const std::string&attempt, int error_code)
        : std::system_error(error_code, category)
        , attempt_and_error_(attempt + ": " + std::system_error::what()){}

    const char * what() const noexcept override { return attempt_and_error_.c_str(); }
private:
    std::string  attempt_and_error_;
};
class util {

};

#endif //TINY_MUDUO_UTIL_H
