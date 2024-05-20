#include "app.h"
#include "config/config.h"
#include "log.h"
#include <cstdlib>

int main(int argc, char *argv[]) {

  if (!Config::configInit()) {
    exit(1);
  }

  auto app = App::create();
  app->getCss("res/ui.css");

  Log::get().setLevel(Log::LevelDebug);
  Log::get().info("Lumastart is now starting...");

  return app->run(argc, argv);
}
