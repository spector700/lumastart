#pragma once

#include "gtkmm/grid.h"
#include "gtkmm/window.h"
#include <iostream>

class Window : public Gtk::Window {

public:
  Window();
  ~Window() override;

protected:
  // Signal handlers:
  void on_button_clicked() { std::cout << "yee\n"; };

  // Child Widgets
  Gtk::Grid m_grid;
};
