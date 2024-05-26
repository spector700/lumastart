#include "window.h"
#include "gdk/gdkkeysyms.h"
#include "giomm/liststore.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtk4-layer-shell.h"
#include "gtkmm/enums.h"
#include "gtkmm/eventcontrollerkey.h"
#include "gtkmm/gestureclick.h"
#include "log.h"
#include "sigc++/functors/mem_fun.h"
#include "src/config/config.h"
#include <iostream>
#include <string>

LumaStart::LumaStart() {
  Log::get().debug("Window Created");

  // Create a 'C' Window object for gtk4-layer-shell
  GtkWindow *c_windowObject = this->gobj();

  // initilize the layer-shell for the window on creation
  gtk_layer_init_for_window(c_windowObject);
  gtk_layer_set_layer(c_windowObject, GTK_LAYER_SHELL_LAYER_OVERLAY);
  gtk_layer_set_namespace(c_windowObject, "lumastart");

  gtk_layer_set_anchor(c_windowObject, GTK_LAYER_SHELL_EDGE_TOP, true);
  gtk_layer_set_anchor(c_windowObject, GTK_LAYER_SHELL_EDGE_BOTTOM, true);
  gtk_layer_set_anchor(c_windowObject, GTK_LAYER_SHELL_EDGE_LEFT, true);
  gtk_layer_set_anchor(c_windowObject, GTK_LAYER_SHELL_EDGE_RIGHT, true);

  // gets the keyboard input
  gtk_layer_set_keyboard_mode(c_windowObject,
                              GTK_LAYER_SHELL_KEYBOARD_MODE_ON_DEMAND);

  // event controller to get the key that is pressed
  auto key_controller = Gtk::EventControllerKey::create(); // keyboard
  auto click_controller = Gtk::GestureClick::create();     // mouse

  m_Grid.set_halign(Gtk::Align::CENTER);
  m_Grid.set_valign(Gtk::Align::CENTER);
  m_Grid.set_expand();
  m_Grid.set_margin_bottom(configSettings.bottom_margin);
  m_Grid.add_css_class("main");

  // set the child of the window
  set_child(m_Grid);

  m_Entry.set_hexpand();
  m_Entry.set_placeholder_text("Search");
  m_Entry.set_search_delay(configSettings.search_delay);
  m_Entry.signal_search_changed().connect(
      sigc::mem_fun(*this, &LumaStart::on_text_changed), false);
  m_Entry.signal_activate().connect(
      sigc::mem_fun(*this, &LumaStart::on_entry_activate), false);

  // attach the entry Widget to the parent grid
  m_Grid.attach(m_Entry, 0, 0);

  m_ListView.set_show_separators();

  m_ScrolledWindow.set_child(m_ListView);
  // have the scroll window match the size of the listview
  m_ScrolledWindow.set_propagate_natural_width(true);
  m_ScrolledWindow.set_propagate_natural_height(true);
  m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER,
                              Gtk::PolicyType::AUTOMATIC);

  m_Revealer.set_child(m_ScrolledWindow);
  m_Grid.attach(m_Revealer, 0, 1);

  key_controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &LumaStart::on_keypress), false);

  click_controller->signal_pressed().connect(
      sigc::mem_fun(*this, &LumaStart::on_clicked), false);

  add_controller(click_controller);
  m_Entry.add_controller(key_controller);

  // Set sizes from config
  m_Grid.set_size_request(configSettings.width);
  m_ScrolledWindow.set_max_content_height(configSettings.height);
  m_ScrolledWindow.set_max_content_width(configSettings.width);

  setupDataModel();
  fillDataModel();
}

void LumaStart::on_entry_activate() {
  auto item = std::dynamic_pointer_cast<Gio::ListModel>(m_ListView.get_model())
                  ->get_object(m_Selection_model->get_selected());

  auto row = std::dynamic_pointer_cast<List>(item);

  g_spawn_command_line_async(row->m_Exec.c_str(), nullptr);
  std::cout << "Launching: " << row->m_Name << '\n';
  close();
}

void LumaStart::on_text_changed() {
  const auto text = m_Entry.get_text();

  if (text.empty()) {
    m_Revealer.set_reveal_child(false);
    return;
  } else {

    m_StringFilter->set_search(text);
    m_Selection_model->set_model(m_FilterListModel);
    // Always keep the first in view when typeing
    m_ListView.scroll_to(0);
    // Select first item
    m_Selection_model->set_selected(0);

    if (!m_Revealer.get_reveal_child()) {
      m_Revealer.set_reveal_child();
    }
  }
}

// keybinds for the window, esc to close and arrow navigation
bool LumaStart::on_keypress(guint keyval, guint, Gdk::ModifierType) {
  if (keyval == GDK_KEY_Escape) {
    close();
    return true;
  }
  // Up arrow or shif + tab
  if (keyval == GDK_KEY_Up || keyval == GDK_KEY_ISO_Left_Tab) {
    if (m_Selection_model->get_selected() == 0) {
      return true;
    }
    // Get the last position at move to it
    guint last = m_Selection_model->get_selected() - 1;
    m_Selection_model->set_selected(last);
    m_ListView.scroll_to(last);
    return true;
  }
  // Down arrow or tab
  if (keyval == GDK_KEY_Down || keyval == GDK_KEY_Tab) {
    // Go to the top of the list if selection is at the bottom
    if (m_Selection_model->get_selected() ==
        m_FilterListModel->property_n_items() - 1) {
      m_Selection_model->set_selected(0);
      m_ListView.scroll_to(0);
      return true;
    }

    // Get the next position and move to it
    guint next = m_Selection_model->get_selected() + 1;
    if (next < m_FilterListModel->property_n_items()) {
      m_Selection_model->set_selected(next);
      m_ListView.scroll_to(next);
      return true;
    }
  }
  return false;
}

// close the window when clicked outside of the main window
void LumaStart::on_clicked(int n_press, double, double) {
  if (n_press == 1) {
    close();
  }
}
