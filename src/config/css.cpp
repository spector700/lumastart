#include "config.h"
#include "gtkmm/cssprovider.h"
#include "gtkmm/styleprovider.h"
#include <iostream>
#include <string>

void Config::loadCss(const std::string &path) {
  /* auto m_refCssProvider =
   * Gtk::make_managed<Glib::RefPtr<Gtk::CssProvider>>(); */
  const auto m_refCssProvider = Gtk::CssProvider::create();

  Gtk::StyleProvider::add_provider_for_display(
      // GTK_STYLE_PROVIDER_PRIORITY_USER allow us to also style the window
      Gdk::Display::get_default(), m_refCssProvider,
      GTK_STYLE_PROVIDER_PRIORITY_USER);

  m_refCssProvider->signal_parsing_error().connect(
      [](const auto &section, const auto &error) {
        on_parsing_error(section, error);
      });

  m_refCssProvider->load_from_path(path);
}

// show error on paring the css file
void Config::on_parsing_error(
    const Glib::RefPtr<const Gtk::CssSection> &section,
    const Glib::Error &error) {
  std::cerr << "Css Parsing Error: " << error.what() << std::endl;
  if (section) {
    const auto file = section->get_file();
    if (file) {
      std::cerr << "  URI = " << file->get_uri() << std::endl;
    }

    auto start_location = section->get_start_location();
    auto end_location = section->get_end_location();
    std::cerr << "  start_line = " << start_location.get_lines() + 1
              << ", end_line = " << end_location.get_lines() + 1 << std::endl;
    std::cerr << "  start_position = " << start_location.get_line_chars()
              << ", end_position = " << end_location.get_line_chars()
              << std::endl;
  }
}
