#pragma once

#include <string>

struct Settings {
  int width{0};
  int height{0};
};

extern Settings configSettings;

namespace Config {
bool configInit();
void parseConfig(const std::string &filepath);

} // namespace Config
