#include "config.h"
#include "giomm/resource.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
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

  // Ensure the directory exists
  std::filesystem::create_directories(
      std::filesystem::path(configPath).parent_path());

  // Get the conf file from the lumastart resource bundle
  const auto source_config =
      Gio::Resource::lookup_data_global("/lumastart/lumastart.conf");

  // Create and open the new config file
  std::ofstream configFile(configPath, std::ios::binary | std::ios::out);

  if (!configFile) {
    std::cerr << "Could not create new config file";
    return false;
  }

  gsize size = 0;
  const gconstpointer data = source_config->get_data(size);

  if (data == nullptr || size == 0) {
    std::cerr << "Failed to get the default config data" << '\n';
    return false;
  }

  if (!configFile.write(reinterpret_cast<const char *>(data),
                        source_config->get_size())) {
    std::cerr << "Failed to write default config data to new file" << '\n';
    configFile.close();
    return false;
  }

  configFile.close();

  std::cout << "Wrote default config to " << configPath << '\n';

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
