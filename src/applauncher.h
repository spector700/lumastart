#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class AppLauncher {
public:
  AppLauncher();

  struct DesktopEntry {
    // for the path values
    std::string path;
    // for the application properties
    std::string name, icon, description, exec;
  };

  static std::unordered_map<std::string,
                            std::unordered_map<std::string, std::string>>
  parseDesktopEntries(std::vector<DesktopEntry> &entries);

  static std::vector<DesktopEntry> getDesktopEntries();
  static std::string getHomeEnv();
};
