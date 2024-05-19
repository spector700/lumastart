#pragma once

#include <string>

#define CONFIG_PATH "/lumastart/lumastart.conf"

struct configfile {
  std::string height;
  std::string width;
};

class Config {

public:
  Config();

  static bool init();

  static void parseConfig(const std::string &filepath);
};
