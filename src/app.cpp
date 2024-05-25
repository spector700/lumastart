#include "app.h"

App::App() : Gtk::Application("com.spector.lumaStart") {}

Glib::RefPtr<App> App::create() {
  return Glib::make_refptr_for_instance<App>(new App());
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
