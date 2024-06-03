#pragma once

#include "glibmm/ustring.h"
#include <string>
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

  struct properties {
    std::string name;
    Glib::ustring description;
    std::string icon;
    std::string exec;
  };

  static std::vector<properties> parseDesktopEntries();
};
