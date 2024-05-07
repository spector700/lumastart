#include "window.h"
#include "applauncher.h"
#include "gdk/gdkkeysyms.h"
#include "giomm/liststore.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtk4-layer-shell.h"
#include "gtkmm/box.h"
#include "gtkmm/enums.h"
#include "gtkmm/eventcontrollerkey.h"
#include "gtkmm/filterlistmodel.h"
#include "gtkmm/gestureclick.h"
#include "gtkmm/image.h"
#include "gtkmm/label.h"
#include "gtkmm/singleselection.h"
#include "gtkmm/stringfilter.h"
#include "log.h"
#include "sigc++/functors/mem_fun.h"
#include <iostream>
#include <string>

Window::Window() : Gtk::Window() {
  Log::get().debug("Window Created");

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

  // event controller to get the key that is pressed
  auto key_controller = Gtk::EventControllerKey::create(); // keyboard
  auto click_controller = Gtk::GestureClick::create();     // mouse

  m_Entry.set_hexpand();
  // use the window key controller to be able to exit on "esc"
  /* m_Entry.add_controller(key_controller); */
  m_Entry.set_placeholder_text("Search");
  m_Entry.signal_changed().connect(
      sigc::mem_fun(*this, &Window::on_text_changed), false);

  // attach the entry Widget to the parent grid
  m_Grid.attach(m_Entry, 0, 0);

  m_ListView.set_show_separators();
  /* m_ScrolledWindow.set_child(m_ListView); */
  /* m_ScrolledWindow.set_max_content_height(400); */
  /* m_ScrolledWindow.set_size_request(200, 200); */
  /* m_ScrolledWindow.set_policy(Gtk::PolicyType::NEVER, */
  /*                             Gtk::PolicyType::AUTOMATIC); */

  m_Revealer.set_child(m_ListView);

  m_Grid.attach(m_Revealer, 0, 1);

  key_controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &Window::on_keypress), false);

  click_controller->signal_pressed().connect(
      sigc::mem_fun(*this, &Window::on_clicked), false);

  add_controller(key_controller);
  add_controller(click_controller);

  setupDataModel();
  fillDataModel();
}

void Window::on_text_changed() {
  const auto text = m_Entry.get_text();
  if (text.empty()) {
    m_Revealer.set_reveal_child(false);
    return;
  } else {
    m_StringFilter->set_search(text);
    m_Selection_model->set_model(m_FilterListModel);
    m_Revealer.set_reveal_child();
    std::cout << "Pressed" << '\n';
  }
}

// close the window, when the 'esc' key is pressed
bool Window::on_keypress(guint keyval, guint, Gdk::ModifierType) {
  if (keyval == GDK_KEY_Escape || keyval == GDK_BUTTON_PRESS) {
    close();
    return true;
  }

  return false;
}

void Window::on_clicked(int n_press, double, double) {
  if (n_press == 1) {
    close();
  }
}

void Window::setupDataModel() {
  m_Data_model = Gio::ListStore<List>::create();
  m_Selection_model = Gtk::SingleSelection::create(m_Data_model);
  m_Selection_model->set_autoselect(true);

  m_Factory = Gtk::SignalListItemFactory::create();
  m_Factory->signal_setup().connect(
      sigc::mem_fun(*this, &Window::on_setup_listitem));
  m_Factory->signal_bind().connect(
      sigc::mem_fun(*this, &Window::on_bind_listitem));

  // Create the filter model.
  auto expression = Gtk::ClosureExpression<Glib::ustring>::create(
      [](const Glib::RefPtr<Glib::ObjectBase> &item) -> Glib::ustring {
        const auto col = std::dynamic_pointer_cast<List>(item);
        return col ? col->m_Name : "";
      });

  m_StringFilter = Gtk::StringFilter::create(expression);
  m_StringFilter->set_ignore_case(true);
  m_StringFilter->set_match_mode(Gtk::StringFilter::MatchMode::SUBSTRING);
  m_FilterListModel =
      Gtk::FilterListModel::create(m_Data_model, m_StringFilter);
}

void Window::fillDataModel() {
  std::vector<AppLauncher::DesktopEntry> de = AppLauncher::getDesktopEntries();
  auto properties = AppLauncher::parseDesktopEntries(de);

  Glib::ustring description;
  std::string icon;

  for (const auto &entry : properties) {
    for (const auto &prop : entry.second) {
      if (prop.first == "Icon") {
        icon = prop.second;

      } else if (prop.first == "Comment") {
        description = prop.second;
      }
    }
    add_entry(entry.first, description, icon);
  }

  m_Data_model->sort(sigc::mem_fun(*this, &Window::on_model_sort));
  m_ListView.set_model(m_Selection_model);
  m_ListView.set_factory(m_Factory);

  m_Selection_model->property_selected().signal_changed().connect(
      sigc::mem_fun(*this, &Window::on_selection_changed));
}

void Window::on_setup_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item) {
  // Each ListItem contains a vertical Box with an Image and a Label.
  auto vBox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  auto icon = Gtk::make_managed<Gtk::Image>();
  icon->set_halign(Gtk::Align::START);
  icon->set_valign(Gtk::Align::CENTER);
  icon->set_icon_size(Gtk::IconSize::LARGE);
  vBox->append(*icon);
  vBox->append(*Gtk::make_managed<Gtk::Label>());
  list_item->set_child(*vBox);
}

void Window::on_bind_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item) {
  auto row = std::dynamic_pointer_cast<List>(list_item->get_item());

  if (!row)
    return;
  auto vBox = dynamic_cast<Gtk::Box *>(list_item->get_child());
  if (!vBox)
    return;
  auto icon = dynamic_cast<Gtk::Image *>(vBox->get_first_child());
  icon->set_from_icon_name(row->m_Icon);
  /* auto app_info = */
  /*     std::dynamic_pointer_cast<Gio::AppInfo>(list_item->get_item()); */
  /* std::cout << "Icon: " << icon << '\n'; */
  /* if (!app_info) { */
  /*   std::cout << "AppInfo: " << app_info << '\n'; */
  /*   return; */
  /* } */
  /* auto image = dynamic_cast<Gtk::Image *>(vBox->get_first_child()); */
  /* image->set(app_info->get_icon()); */

  auto label = dynamic_cast<Gtk::Label *>(vBox->get_last_child());
  if (!label) {
    std::cerr << "No label\n";
    return;
  }

  label->set_text(row->m_Name);
}

void Window::on_selection_changed() {
  auto position = m_Selection_model->get_selected();
  auto col = m_Data_model->get_item(position);

  if (!col)
    return;

  const std::string name = col->m_Name;
  const Glib::ustring description = col->m_Description;

  std::cout << "Selection Changed to: name=" << name
            << ", description=" << description << std::endl;
  /* auto item = m_Selection_model->get_selected_item(); */
  /* auto row = std::dynamic_pointer_cast<Gtk::TreeListRow>(item); */
  /* if (!row) */
  /*   return; */
  /* auto col = std::dynamic_pointer_cast<List>(row->get_item()); */
  /* if (!col) */
  /*   return; */
  /* set_title(col->m_Name); */
  /* if (!col->m_filename.empty()) */
  /*   load_file(col->m_filename); */
}

int Window::on_model_sort(const Glib::RefPtr<const List> &a,
                          const Glib::RefPtr<const List> &b) {
  return (a->m_Name < b->m_Name) ? -1 : ((a->m_Name > b->m_Name) ? 1 : 0);
}

void Window::add_entry(const std::string &name,
                       const Glib::ustring &description,
                       const std::string &icon) {

  m_Data_model->append(List::create(name, description, icon));
}
