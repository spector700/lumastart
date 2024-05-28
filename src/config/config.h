#pragma once

#include "glibmm/error.h"
#include "glibmm/refptr.h"
#include "gtkmm/csssection.h"
#include <string>

#define CONFIG_DIR "/.config/lumastart/"
#define CONFIG_PATH "lumastart.conf"
#define CSS_PATH "style.css"

struct Settings {
  int width{0};
  int height{0};
  int bottom_margin{0};
  int search_delay{0};
};

extern Settings configSettings;

namespace Config {
bool configInit();
void parseConfig(const std::string &filepath);
void loadCss(const std::string &path);
void on_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section,
                      const Glib::Error &error);
} // namespace Config
