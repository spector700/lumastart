#include "app.h"
#include "log.h"

int main(int argc, char *argv[]) {
  auto app = App::create();
  app->getCss("src/ui.css");

  Log log;
  log.SetLevel(Log::LevelDebug);
  log.Info("Lumastart is now starting...");

  return app->run(argc, argv);
}
