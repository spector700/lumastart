#include "config.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#define CONFIG_PATH "/lumastart/lumastart.conf"

Settings configSettings;

bool Config::configInit() {
  const char *home = std::getenv("XDG_CONFIG_HOME");
  const std::string configPath = std::string(home) + CONFIG_PATH;

  // Check if the config file already exists
  if (std::filesystem::exists(configPath)) {
    std::cout << "Config file already exists.\n";
    Config::parseConfig(configPath);
    return true;
  }

  if (!home || !std::filesystem::path(home).is_absolute()) {
    std::cout << "Home could not be found" << '\n';
    return false;
  }

  std::cout << "Config file does not exist" << '\n';
  std::cout << "Creating default config..." << '\n';

  // Check if the source file exists
  if (!std::filesystem::exists("res/lumastart.conf")) {
    std::cerr << "Source file does not exist.\n";
    return false;
  }

  // Ensure the directory exists
  std::filesystem::create_directories(
      std::filesystem::path(configPath).parent_path());

  if (std::filesystem::copy_file("res/lumastart.conf", configPath)) {
    std::cout << "Copied default config file to " << configPath << '\n';
  } else {
    std::cerr << "Could not copy default file to " << configPath << '\n';
    return false;
  }

  Config::parseConfig(configPath);
  return true;
}

void Config::parseConfig(const std::string &filepath) {
  std::ifstream config(filepath);

  if (!config.is_open()) {
    std::cerr << "Could not open config file to read: " << filepath << '\n';
    exit(1);
  }

  std::string line;
  // read the file by each line
  while (std::getline(config, line)) {
    // Ignore comments
    if (line.starts_with('#') || line.empty()) {
      continue;
    }

    // seperate the key value at the "="
    size_t pos = line.find('=');
    if (pos != std::string::npos) {
      std::string key = line.substr(0, pos - 1);
      std::string value = line.substr(pos + 2);

      if (key == "width") {
        configSettings.width = std::stoi(value);
      }
      if (key == "height") {
        configSettings.height = std::stoi(value);
      }
    }
  }

  config.close();
}
