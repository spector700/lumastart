#pragma once

#include "gtkmm/grid.h"
#include "gtkmm/window.h"
#include "src/log.h"

class Window : public Gtk::Window {

public:
  Window();
  /* ~Window() override; */
  ~Window() {
    Log log;
    log.Debug("Window Destroyed");
  }

protected:
  // Signal handlers:
  bool on_window_key_pressed(guint keyval, guint keycode,
                             Gdk::ModifierType state);
  void outside_window_clicked(){};

  // Child Widgets
  Gtk::Grid m_Grid;
};
