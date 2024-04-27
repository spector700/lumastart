#pragma once

#include <iostream>

class Log {

public:
  enum Level {
    LevelError = 0,
    LevelWaring = 1,
    LevelInfo = 2,
  };

private:
  Level m_LogLevel{LevelInfo};

public:
  void SetLevel(Level level) { m_LogLevel = level; }

  void Info(const char *message) {
    if (m_LogLevel >= LevelInfo) {
      std::cout << "[Info]: " << message << '\n';
    }
  }

  void Warn(const char *message) {
    if (m_LogLevel >= LevelWaring) {
      std::cout << "[Warning]: " << message << '\n';
    }
  }

  void Error(const char *message) {
    if (m_LogLevel >= LevelError) {
      std::cout << "[ERROR]: " << message << '\n';
    }
  }
};
