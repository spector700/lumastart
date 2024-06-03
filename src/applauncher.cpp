#include "applauncher.h"
#include "giomm/desktopappinfo.h"
#include "glibmm/ustring.h"
#include <memory>
#include <string>
#include <vector>

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
}
