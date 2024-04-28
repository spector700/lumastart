#include "gtkmm/application.h"
#include "src/log.h"
#include "window.h"

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.lumastart");

  Log log;
  log.SetLevel(Log::LevelDebug);
  log.Info("Lumastart is now starting...");

  return app->make_window_and_run<Window>(argc, argv);
}
