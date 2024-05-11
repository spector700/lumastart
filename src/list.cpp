#include "applauncher.h"
#include "giomm/appinfo.h"
#include "gtkmm/enums.h"
#include "gtkmm/filterlistmodel.h"
#include "gtkmm/grid.h"
#include "gtkmm/image.h"
#include "gtkmm/label.h"
#include "gtkmm/object.h"
#include "gtkmm/singleselection.h"
#include "gtkmm/stringfilter.h"
#include "sigc++/functors/mem_fun.h"
#include "window.h"
#include <iostream>
#include <memory>
#include <vector>

void LumaStart::setupDataModel() {
  m_Data_model = Gio::ListStore<List>::create();
  m_Selection_model = Gtk::SingleSelection::create(m_Data_model);

  m_Factory = Gtk::SignalListItemFactory::create();
  m_Factory->signal_setup().connect(
      sigc::mem_fun(*this, &LumaStart::on_setup_listitem));
  m_Factory->signal_bind().connect(
      sigc::mem_fun(*this, &LumaStart::on_bind_listitem));

  m_ListView.signal_activate().connect(
      sigc::mem_fun(*this, &LumaStart::on_item_activated));

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

void LumaStart::fillDataModel() {
  std::vector<AppLauncher::DesktopEntry> de = AppLauncher::getDesktopEntries();
  std::vector<AppLauncher::properties> properties =
      AppLauncher::parseDesktopEntries(de);

  Glib::ustring description;
  std::string icon;
  std::string exec;

  for (const auto &entry : properties) {
    List::properties entryProperties;
    entryProperties.name = entry.name;
    entryProperties.description = entry.description;
    entryProperties.icon = entry.icon;
    entryProperties.exec = entry.exec;

    add_entry(entryProperties);
  }

  m_Data_model->sort(sigc::mem_fun(*this, &LumaStart::on_model_sort));
  m_ListView.set_model(m_Selection_model);
  m_ListView.set_factory(m_Factory);

  m_Selection_model->property_selected().signal_changed().connect(
      sigc::mem_fun(*this, &LumaStart::on_selection_changed));
}

void LumaStart::on_setup_listitem(
    const Glib::RefPtr<Gtk::ListItem> &list_item) {
  // Each ListItem contains a vertical Box with an Image and a Label.
  auto grid = Gtk::make_managed<Gtk::Grid>();
  auto icon = Gtk::make_managed<Gtk::Image>();
  auto name = Gtk::make_managed<Gtk::Label>();
  auto description = Gtk::make_managed<Gtk::Label>();

  icon->set_valign(Gtk::Align::CENTER);
  icon->set_halign(Gtk::Align::CENTER);
  icon->set_icon_size(Gtk::IconSize::LARGE);
  icon->set_vexpand();
  icon->set_margin(10);
  grid->attach(*icon, 0, 0, 1, 2);

  name->set_valign(Gtk::Align::CENTER);
  name->set_halign(Gtk::Align::START);
  name->set_expand();
  name->set_margin_start(5);
  grid->attach(*name, 1, 0);

  description->set_valign(Gtk::Align::CENTER);
  description->set_halign(Gtk::Align::START);
  description->set_expand();
  description->set_margin_start(5);
  grid->attach(*description, 1, 1);

  grid->set_size_request(0, 60);
  list_item->set_child(*grid);
}

void LumaStart::on_bind_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item) {
  auto row = std::dynamic_pointer_cast<List>(list_item->get_item());

  if (!row)
    return;

  auto grid = dynamic_cast<Gtk::Grid *>(list_item->get_child());
  if (!grid) {
    std::cerr << "No grid\n";
    return;
  }

  auto icon = dynamic_cast<Gtk::Image *>(grid->get_child_at(0, 0));
  if (!icon) {
    std::cerr << "No icon\n";
    return;
  }

  icon->set_from_icon_name(row->m_Icon);

  auto name = dynamic_cast<Gtk::Label *>(grid->get_child_at(1, 0));
  if (!name) {
    std::cerr << "No name label\n";
    return;
  }

  name->set_text(row->m_Name);

  auto description = dynamic_cast<Gtk::Label *>(grid->get_child_at(1, 1));
  if (!description) {
    std::cerr << "No description label\n";
    return;
  }

  description->set_text(row->m_Description);
}

void LumaStart::on_selection_changed() {
  auto position = m_Selection_model->get_selected();
  auto col = m_Data_model->get_item(position);

  if (!col)
    return;
}

void LumaStart::on_item_activated(unsigned int position) {
  auto item = std::dynamic_pointer_cast<Gio::ListModel>(m_ListView.get_model())
                  ->get_object(position);

  auto row = std::dynamic_pointer_cast<List>(item);
  auto app_info = std::dynamic_pointer_cast<Gio::AppInfo>(item);

  std::cout << "item: " << row->m_Exec << '\n';

// run the execution
#if 0

  gint exit_status = g_spawn_command_line_async(row->m_Exec.c_str(), nullptr);
  close();

  if (exit_status == 0) {
    std::cout << "Command executed successfully." << '\n';
  } else {
    std::cerr << "Command execution failed with exit status: " << exit_status
              << '\n';
  }
#endif // 0
}

int LumaStart::on_model_sort(const Glib::RefPtr<const List> &a,
                             const Glib::RefPtr<const List> &b) {
  return (a->m_Name < b->m_Name) ? -1 : ((a->m_Name > b->m_Name) ? 1 : 0);
}

void LumaStart::add_entry(const List::properties &prop) {

  m_Data_model->append(
      List::create(prop.name, prop.description, prop.icon, prop.exec));
}
