#pragma once

#include "giomm/liststore.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "gtkmm/filterlistmodel.h"
#include "gtkmm/grid.h"
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

class LumaStart : public Gtk::Window {
public:
  LumaStart();

protected:
  class List;
  // signal handlers
  void on_item_activated(unsigned int position);
  void on_selection_changed();
  void on_setup_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item);
  void on_bind_listitem(const Glib::RefPtr<Gtk::ListItem> &list_item);
  int on_model_sort(const Glib::RefPtr<const List> &a,
                    const Glib::RefPtr<const List> &b);
  void add_entry(const std::string &name, const Glib::ustring &description,
                 const std::string &icon);

  // A Gio::ListStore stores filename, description and texture.
  class List : public Glib::Object {
  public:
    std::string m_Name;
    Glib::ustring m_Description;
    std::string m_Icon;

    static Glib::RefPtr<List> create(const std::string &name,
                                     const Glib::ustring &description,
                                     const std::string &icon) {
      return Glib::make_refptr_for_instance<List>(
          new List(name, description, icon));
    }

  protected:
    List(const std::string &name, const Glib::ustring &description,
         const std::string &icon)
        : m_Name(name), m_Description(description), m_Icon(icon) {}
  }; // List

  Glib::RefPtr<Gio::ListStore<List>> m_Data_model;
  Glib::RefPtr<Gtk::SingleSelection> m_Selection_model;
  Glib::RefPtr<Gtk::SignalListItemFactory> m_Factory;
  Glib::RefPtr<Gtk::FilterListModel> m_FilterListModel;
  Glib::RefPtr<Gtk::StringFilter> m_StringFilter;

  void setupDataModel();
  void fillDataModel();
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
