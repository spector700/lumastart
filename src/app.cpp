#include "app.h"
#include <iostream>

App::App() : Gtk::Application("com.spector.lumaStart") {}

Glib::RefPtr<App> App::create() {
  return Glib::make_refptr_for_instance<App>(new App());
}

// read the css file
void App::getCss(const std::string &file) {

  m_refCssProvider = Gtk::CssProvider::create();

  Gtk::StyleProvider::add_provider_for_display(
      // GTK_STYLE_PROVIDER_PRIORITY_USER allow us to also style the window
      Gdk::Display::get_default(), m_refCssProvider,
      GTK_STYLE_PROVIDER_PRIORITY_USER);

  m_refCssProvider->signal_parsing_error().connect(
      [](const auto &section, const auto &error) {
        on_parsing_error(section, error);
      });

  m_refCssProvider->load_from_resource(file);

  std::cout << "Loaded Css: " << file << '\n';
}

LumaStart *App::create_window() {
  auto window = new LumaStart();

  add_window(*window);

  window->signal_hide().connect([window]() { delete window; });

  return window;
}

// create and show the window
void App::on_activate() {
  auto window = create_window();
  window->present();
}

// show error on paring the css file
void App::on_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section,
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
