#include "app.h"
#include "log.h"

int main(int argc, char *argv[]) {
  auto app = App::create();
  app->getCss("res/ui.css");

  Log::get().setLevel(Log::LevelDebug);
  Log::get().info("Lumastart is now starting...");

  return app->run(argc, argv);
}
