#pragma once

#include <iostream>
#include <string>

// singleton log class
class Log {

public:
  enum Level {
    LevelError = 0,
    LevelWaring = 1,
    LevelInfo = 2,
    LevelDebug = 3,
  };

private:
  Level m_LogLevel{LevelInfo};
  Log() {}

public:
  // Static method to get the instance of the Log class
  static Log &get() {
    static Log instance;
    return instance;
  }

  void setLevel(Level level) { m_LogLevel = level; }
  int getLevel() { return m_LogLevel; }

  void debug(const std::string &message) {
    if (m_LogLevel == LevelDebug) {
      std::cout << "[DEBUG]: " << message << '\n';
    }
  }

  void info(const char *message) {
    if (m_LogLevel >= LevelInfo) {
      std::cout << "[INFO]: " << message << '\n';
    }
  }

  void warn(const char *message) {
    if (m_LogLevel >= LevelWaring) {
      std::cerr << "[WARNING]: " << message << '\n';
    }
  }

  void error(const char *message) {
    if (m_LogLevel >= LevelError) {
      std::cerr << "[ERROR]: " << message << '\n';
    }
  }
};
