#include "window.h"
#include "gdkmm/enums.h"
#include "gtk4-layer-shell.h"
#include "gtkmm/entry.h"
#include "gtkmm/eventcontrollerkey.h"
#include "sigc++/functors/mem_fun.h"

Window::Window() {

  Log log;
  log.SetLevel(Log::LevelDebug);
  log.Debug("Window Created");

  // make the current window a gobject and create a variable
  auto window = this->gobj();

  // initilize the layer-shell for the window on creation
  gtk_layer_init_for_window(window);
  gtk_layer_set_layer(window, GTK_LAYER_SHELL_LAYER_TOP);

  // gets the keyboard input
  gtk_layer_set_keyboard_mode(window, GTK_LAYER_SHELL_KEYBOARD_MODE_ON_DEMAND);

  m_Grid.set_margin(10);
  m_Grid.set_expand();
  m_Grid.set_size_request(650, 10);

  // set the child of the window
  set_child(m_Grid);

  // Create the text entry field
  auto m_Entry = Gtk::make_managed<Gtk::Entry>();
  m_Entry->set_hexpand();
  m_Entry->set_icon_from_icon_name("system-search-symbolic");
  m_Entry->set_placeholder_text("Search");

  // attach the entry Widget to the parent grid
  m_Grid.attach(*m_Entry, 1, 1);

  // event controller to get the key that is pressed
  auto controller = Gtk::EventControllerKey::create();
  controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &Window::on_window_key_pressed), false);
  add_controller(controller);
}

bool Window::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType) {
  // close the window, when the 'esc' key is pressed
  if (keyval == GDK_KEY_Escape) {
    close();
    return true;
  }
  return false;
}
