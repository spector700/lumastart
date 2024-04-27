#include "gtk4-layer-shell.h"
#include "gtkmm/application.h"
#include "gtkmm/button.h"
#include "gtkmm/object.h"
#include "log.h"
#include "sigc++/functors/mem_fun.h"
#include "window.h"

Window::Window() {
  m_grid.set_margin(10);

  gtk_layer_init_for_window(this->gobj());

  auto m_button = Gtk::make_managed<Gtk::Button>("hyy");

  m_grid.attach(*m_button, 3, 2);

  m_button->signal_clicked().connect(
      sigc::mem_fun(*this, &Window::on_button_clicked));

  set_child(m_grid);
}

Window::~Window() {}

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.examples.base");

  Log log;
  log.Info("Lumastart is now starting...");

  return app->make_window_and_run<Window>(argc, argv);
}
