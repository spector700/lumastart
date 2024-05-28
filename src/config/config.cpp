#include "config.h"
#include "utils/resource.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

Settings configSettings;

bool Config::configInit() {
  const char *home = std::getenv("HOME");
  const std::string configDir = std::string(home) + CONFIG_DIR;
  const std::string configPath = configDir + CONFIG_PATH;
  const std::string cssPath = configDir + CSS_PATH;

  if (!home || !std::filesystem::path(home).is_absolute()) {
    std::cout << "Home could not be found" << '\n';
    return false;
  }

  // Ensure the directory exists
  std::filesystem::create_directories(
      std::filesystem::path(configPath).parent_path());

  // Check if the config file already exists
  if (std::filesystem::exists(configPath)) {
    std::cout << "Config file already exists.\n";
    Config::parseConfig(configPath);

  } else {
    std::cout << "Config file does not exist" << '\n';
    std::cout << "Creating default config..." << '\n';

    const auto source_config = Utils::getSourceFile(CONFIG_PATH);
    Utils::writeSourceFile(source_config, configPath);

    Config::parseConfig(configPath);
  }
  std::cout << "Read Config file at " << configPath << '\n';

  // Check if the style.css file already exists
  if (std::filesystem::exists(cssPath)) {
    std::cout << "Css file already exists.\n";
    Config::loadCss(cssPath);

  } else {
    std::cout << "Style file does not exist" << '\n';
    std::cout << "Creating default style.css..." << '\n';

    const auto source_css = Utils::getSourceFile(CSS_PATH);
    Utils::writeSourceFile(source_css, cssPath);

    Config::loadCss(cssPath);
  }
  std::cout << "Read Css file at " << cssPath << '\n';

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
      if (key == "bottom_margin") {
        configSettings.bottom_margin = std::stoi(value);
      }
      if (key == "search_delay") {
        configSettings.search_delay = std::stoi(value);
      }
    }
  }
  config.close();
}
