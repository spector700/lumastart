#include "applauncher.h"
#include "giomm/desktopappinfo.h"
#include "glibmm/ustring.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

std::string AppLauncher::getHomeEnv() {
  // get the user xdg applications directory from the XDG_DATA_HOME env variable
  const char *xdg_home = std::getenv("XDG_DATA_HOME");

  if (xdg_home != nullptr) {

    std::string xdg_user_home = std::string(xdg_home) + "/applications";
    std::cerr << xdg_user_home << '\n';
    return xdg_user_home;

  } else {

    // if XDG_DATA_HOME does not exist, then use HOME
    const char *xdg_home = std::getenv("HOME");
    if (xdg_home != nullptr) {
      std::string xdg_user_home = std::string(xdg_home) + "/applications";
      std::cerr << "Using HOME: " << xdg_user_home << '\n';
      return xdg_user_home;

    } else {
      std::cerr << "Unable to get the Home environment variable" << '\n';
      return "";
    }
  }
}

std::vector<AppLauncher::DesktopEntry> AppLauncher::getDesktopEntries() {
  const char *xdg_data_char = std::getenv("XDG_DATA_DIRS");

  if (xdg_data_char != nullptr) {
    std::string xdg_data_dirs(xdg_data_char);
    std::vector<std::string> paths;

    unsigned int pos{0};
    // split the directories at ":" and put them in paths
    while (pos < xdg_data_dirs.size()) {
      pos = xdg_data_dirs.find(":");
      paths.push_back(xdg_data_dirs.substr(0, pos));

      // 1 is the size of ":"
      xdg_data_dirs.erase(0, pos + 1);
    }

    std::vector<DesktopEntry> entries;

    // loop through the directories
    for (const auto &path : paths) {
      std::filesystem::path dir(path);
      try {
        dir = path + "/applications/";

        // if the directory that has a "/applications/" exists
        if (std::filesystem::exists(dir)) {

#ifdef DEBUG
          std::cout << "directory: " << dir << '\n';
#endif

          // loop through the valid directories and add the applications to the
          // entries vector
          for (const auto &entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
              DesktopEntry desktopFile;

              desktopFile.path = entry.path();

              // fill the vector
              entries.push_back(desktopFile);
#ifdef DEBUG
              std::cout << "Name: " << desktopFile.name << '\n';
              std::cout << "Path: " << desktopFile.path << '\n';
#endif
            }
          }
        }

      } catch (std::filesystem::filesystem_error &e) {
        std::cerr << "No such file or directory" << e.what() << '\n';
        continue;
      }
    }
    return entries;
  }
  return {};
}

// reads all the desktop entry files and returns a struct of the properties
std::vector<AppLauncher::properties> AppLauncher::parseDesktopEntries() {
  std::vector<properties> entriesVector;

  auto apps = Gio::DesktopAppInfo::get_all();

  for (const auto &app : apps) {
    properties entryProperties;
    auto desktop = std::dynamic_pointer_cast<Gio::DesktopAppInfo>(app);

    // if the entry should be shown in menus, like for app launchers
    if (desktop->should_show()) {

      entryProperties.name = desktop->get_name();
      entryProperties.description = desktop->get_description();

      Glib::RefPtr<Gio::Icon> icon = desktop->get_icon();
      if (icon) {
        entryProperties.icon = icon->to_string();
      }
      entryProperties.exec = desktop->get_executable();

      entriesVector.push_back(entryProperties);
    }
  }
  return entriesVector;
#if 0
  
  for (auto &entry : entries) {
    properties entryProperties;
    // load the file from the path
    std::ifstream file(entry.path);
    if (!file.is_open()) {
      continue;
    }

    std::string line;
    bool header_found{false};

    // read the file by each line
    while (std::getline(file, line)) {
      // check if its the header of the file
      if (line == "[Desktop Entry]" && !header_found) {
        header_found = true;
        continue;
        // exit the file if the main entry has already been paresd to avoid
        // getting the app actions
      } else if ((line[0] == '[' && line.ends_with(']')) && header_found) {
        break;
      }

      else {
        // seperate the key value at the "="
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
          std::string key = line.substr(0, pos);
          std::string value = line.substr(pos + 1);

          if (key == "Name") {
            entryProperties.name = value;
          } else if (key == "Comment") {
            entryProperties.description = value;
          } else if (key == "Icon") {
            entryProperties.icon = value;
          } else if (key == "Exec") {
            entryProperties.exec = value;
          }
        }
      }
    }
    entriesVector.push_back(entryProperties);
  }
  return entriesVector;
#endif // 0
}
