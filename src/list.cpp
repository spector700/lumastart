#include "applauncher.h"
#include "gtkmm/box.h"
#include "gtkmm/filterlistmodel.h"
#include "gtkmm/image.h"
#include "gtkmm/label.h"
#include "gtkmm/singleselection.h"
#include "gtkmm/stringfilter.h"
#include "window.h"
#include <iostream>

void LumaStart::setupDataModel() {
  m_Data_model = Gio::ListStore<List>::create();
  m_Selection_model = Gtk::SingleSelection::create(m_Data_model);
  m_Selection_model->set_autoselect(true);

  m_Factory = Gtk::SignalListItemFactory::create();
  m_Factory->signal_setup().connect(
      sigc::mem_fun(*this, &LumaStart::on_setup_listitem));
  m_Factory->signal_bind().connect(
      sigc::mem_fun(*this, &LumaStart::on_bind_listitem));

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

  m_Data_model->sort(sigc::mem_fun(*this, &LumaStart::on_model_sort));
  m_ListView.set_model(m_Selection_model);
  m_ListView.set_factory(m_Factory);

  m_Selection_model->property_selected().signal_changed().connect(
      sigc::mem_fun(*this, &LumaStart::on_selection_changed));
}

void LumaStart::on_setup_listitem(
    const Glib::RefPtr<Gtk::ListItem> &list_item) {
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

void LumaStart::on_bind_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item) {
  auto row = std::dynamic_pointer_cast<List>(list_item->get_item());

  if (!row)
    return;

  auto vBox = dynamic_cast<Gtk::Box *>(list_item->get_child());
  if (!vBox) {
    std::cerr << "No vBox\n";
    return;
  }

  auto icon = dynamic_cast<Gtk::Image *>(vBox->get_first_child());
  if (!icon) {
    std::cerr << "No icon\n";
    return;
  }

  icon->set_from_icon_name(row->m_Icon);

  auto label = dynamic_cast<Gtk::Label *>(vBox->get_last_child());
  if (!label) {
    std::cerr << "No label\n";
    return;
  }

  label->set_text(row->m_Name);
}

void LumaStart::on_selection_changed() {
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

int LumaStart::on_model_sort(const Glib::RefPtr<const List> &a,
                             const Glib::RefPtr<const List> &b) {
  return (a->m_Name < b->m_Name) ? -1 : ((a->m_Name > b->m_Name) ? 1 : 0);
}

void LumaStart::add_entry(const std::string &name,
                          const Glib::ustring &description,
                          const std::string &icon) {

  m_Data_model->append(List::create(name, description, icon));
}
