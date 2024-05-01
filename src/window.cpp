#include "window.h"
#include "gdk/gdkkeysyms.h"
#include "gdkmm/enums.h"
#include "glibmm/ustring.h"
#include "gtk4-layer-shell.h"
#include "gtkmm/entry.h"
#include "gtkmm/enums.h"
#include "gtkmm/eventcontrollerkey.h"
#include "sigc++/functors/mem_fun.h"
#include "src/log.h"
#include <iostream>

Window::Window() : Gtk::Window() {
  Log log;
  log.SetLevel(Log::LevelDebug);

  log.Debug("Window Created");

  // initilize the layer-shell for the window on creation
  gtk_layer_init_for_window(this->gobj());
  gtk_layer_set_layer(this->gobj(), GTK_LAYER_SHELL_LAYER_OVERLAY);
  gtk_layer_set_namespace(this->gobj(), "lumastart");

  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_TOP, true);
  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_BOTTOM, true);
  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_LEFT, true);
  gtk_layer_set_anchor(this->gobj(), GTK_LAYER_SHELL_EDGE_RIGHT, true);

  gtk_layer_set_exclusive_zone(this->gobj(), -1);

  // gets the keyboard input
  gtk_layer_set_keyboard_mode(this->gobj(),
                              GTK_LAYER_SHELL_KEYBOARD_MODE_ON_DEMAND);

  m_Grid.set_halign(Gtk::Align::CENTER);
  m_Grid.set_valign(Gtk::Align::CENTER);
  m_Grid.set_size_request(650, 30);
  m_Grid.add_css_class("grid");

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
      sigc::mem_fun(*this, &Window::on_keypress), false);

  add_controller(controller);
}

// close the window, when the 'esc' key is pressed
bool Window::on_keypress(guint keyval, guint, Gdk::ModifierType) {
  if (keyval == GDK_KEY_Escape) {
    close();
    return true;
  }

  std::cout << keyval << '\n';

  return false;
}
