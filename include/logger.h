//
// Created by yngty on 2023/3/4.
//

#ifndef TINY_MUDUO_LOGGER_H
#define TINY_MUDUO_LOGGER_H

#include "noncopyable.h"
#include <string>

#define LOG_INFO(logmsgFormat, ...)                                            \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(LogLevel::INFO);                                        \
    char buf[1024]{};                                                          \
    snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                          \
    logger.log(buf);                                                           \
  } while (0)

#define LOG_ERROR(logmsgFormat, ...)                                           \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(LogLevel::ERROR);                                       \
    char buf[1024]{};                                                          \
    snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                          \
    logger.log(buf);                                                           \
  } while (0)

#define LOG_FATAL(logmsgFormat, ...)                                           \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(LogLevel::FATAL);                                       \
    char buf[1024] = {0};                                                      \
    snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                          \
    logger.log(buf);                                                           \
    exit(-1);                                                                  \
  } while (0)
#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...)                                           \
  do {                                                                         \
    Logger &logger = Logger::instance();                                       \
    logger.setLogLevel(LogLevel::DEBUG);                                       \
    char buf[1024] = {0};                                                      \
    snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                          \
    logger.log(buf);                                                           \
  } while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif

enum class LogLevel { INFO, ERROR, FATAL, DEBUG };

class Logger : Noncopyable {
public:
  static Logger &instance();
  void setLogLevel(LogLevel level) { logLevel_ = level; }
  void log(std::string msg);

private:
  LogLevel logLevel_;
};

#endif // TINY_MUDUO_LOGGER_H
