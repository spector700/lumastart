#include "config.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

bool Config::init() {
  const char *home = std::getenv("XDG_CONFIG_HOME");
  const std::string configPath = std::string(home) + CONFIG_PATH;

  if (std::filesystem::exists(configPath)) {
    std::cout << "Config file already exists.\n";
    return true;
  }

  if (!home || !std::filesystem::path(home).is_absolute()) {
    std::cout << "Home could not be found" << '\n';
    return false;
  }

  std::cout << "Config file does not exist" << '\n';
  std::cout << "Creating default config..." << '\n';

  // Check if the source file exists
  if (!std::filesystem::exists("src/config/lumastart.conf")) {
    std::cerr << "Source file does not exist.\n";
    return false;
  }

  // Ensure the directory exists
  std::filesystem::create_directories(
      std::filesystem::path(configPath).parent_path());

  if (std::filesystem::copy_file("src/config/lumastart.conf", configPath)) {
    std::cout << "Copied default config file to " << configPath << '\n';
  } else {
    std::cerr << "Could not copy default file to " << configPath << '\n';
  }

  return true;
}

void Config::parseConfig(const std::string &filepath) {}
