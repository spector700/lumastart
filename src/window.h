#pragma once

#include "gtkmm/grid.h"
#include "gtkmm/window.h"

class Window : public Gtk::Window {
public:
  Window();

protected:
  bool on_keypress(guint keyval, guint keycode, Gdk::ModifierType state);

private:
  Gtk::Grid m_Grid;
};
