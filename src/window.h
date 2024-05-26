#pragma once

#include "giomm/liststore.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtkmm/filterlistmodel.h"
#include "gtkmm/grid.h"
#include "gtkmm/icontheme.h"
#include "gtkmm/listitem.h"
#include "gtkmm/listview.h"
#include "gtkmm/revealer.h"
#include "gtkmm/scrolledwindow.h"
#include "gtkmm/searchentry.h"
#include "gtkmm/signallistitemfactory.h"
#include "gtkmm/singleselection.h"
#include "gtkmm/stringfilter.h"
#include "gtkmm/window.h"
#include <string>

/**
 * @class LumaStart
 * @brief Represents the main window of the application, handling UI elements
 * and data management.
 *
 * This class manages the application's main window, including UI components
 * like a grid, revealers, scrolled windows, list views, and search entries. It
 * also handles data models, selection models, and list item factories for
 * managing the list view's content.
 */
class LumaStart : public Gtk::Window {
public:
  LumaStart();

protected:
  // Nested List class for managing list items.
  class List;

  // A Gio::ListStore stores filename, description and texture.
  class List : public Glib::Object {
  public:
    /**
     * @struct properties
     * @brief Struct representing the properties of a list item.
     */
    struct properties {
      std::string name;
      Glib::ustring description;
      std::string icon;
      std::string exec;
    };

    std::string m_Name;
    Glib::ustring m_Description;
    std::string m_Icon;
    std::string m_Exec;

    /**
     * @brief Static factory method for creating List instances.
     *
     * Constructs a new List object with the given properties.
     */
    static Glib::RefPtr<List> create(const std::string &name,
                                     const Glib::ustring &description,
                                     const std::string &icon,
                                     const std::string &exec) {
      properties properties = {name, description, icon, exec};
      return Glib::make_refptr_for_instance<List>(new List(properties));
    }

  protected:
    List(const properties &properties)
        : m_Name(properties.name), m_Description(properties.description),
          m_Icon(properties.icon), m_Exec(properties.exec) {}
  }; // List

  // signal handlers
  void on_item_activated(unsigned int position);
  void on_selection_changed();
  void on_setup_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item);
  void on_bind_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item);
  int on_model_sort(const Glib::RefPtr<const List> &a,
                    const Glib::RefPtr<const List> &b);
  void add_entry(const List::properties &prop);

  Glib::RefPtr<Gio::ListStore<List>> m_Data_model;
  Glib::RefPtr<Gtk::SingleSelection> m_Selection_model;
  Glib::RefPtr<Gtk::SignalListItemFactory> m_Factory;
  Glib::RefPtr<Gtk::FilterListModel> m_FilterListModel;
  Glib::RefPtr<Gtk::StringFilter> m_StringFilter;
  Glib::RefPtr<Gtk::IconTheme> m_IconTheme;

  void setupDataModel();
  void fillDataModel();
  void on_entry_activate();
  void on_text_changed();
  bool on_keypress(guint keyval, guint keycode, Gdk::ModifierType state);
  void on_clicked(int n_press, double x, double y);

private:
  // child widgets
  Gtk::Grid m_Grid;
  Gtk::Revealer m_Revealer;
  Gtk::ScrolledWindow m_ScrolledWindow;
  Gtk::ListView m_ListView;
  Gtk::SearchEntry m_Entry;
};
