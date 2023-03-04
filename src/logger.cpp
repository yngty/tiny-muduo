//
// Created by yngty on 2023/3/4.
//

#include "logger.h"
#include "timestamp.h"
#include <iostream>

Logger &Logger::instance() {
  static Logger logger{};
  return logger;
}

void Logger::log(std::string msg) {
  switch (logLevel_) {
  case LogLevel::INFO:
    std::cout << "[INFO]";
    break;
  case LogLevel::ERROR:
    std::cout << "[ERROR]";
    break;
  case LogLevel::FATAL:
    std::cout << "[FATAL]";
    break;
  case LogLevel::DEBUG:
    std::cout << "[DEBUG]";
    break;
  default:
    break;
  }
  std::cout << Timestamp::now().to_string() << " " << msg << std::endl;
}