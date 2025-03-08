#include "config/config.h"
#include "glibmm/refptr.h"
#include "gtkmm/application.h"
#include "log.h"
#include "src/window.h"
#include <cstdlib>

int main(int argc, char *argv[]) {
  Log::get().setLevel(Log::LevelDebug);
  Glib::RefPtr<Gtk::Application> app =
      Gtk::Application::create("com.spector.lumaStart");

  app->register_application();

  if (app->is_remote()) {
    Log::get().error("An instance of Lumastart is already running");
    return 0;
  }

  if (!Config::configInit()) {
    exit(1);
  }

  Log::get().info("Lumastart is now starting...");

  return app->make_window_and_run<LumaStart>(argc, argv);
}
