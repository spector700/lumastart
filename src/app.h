#include "glibmm/refptr.h"
#include "gtkmm/application.h"
#include "gtkmm/cssprovider.h"
#include "gtkmm/window.h"
#include "window.h"

class App : public Gtk::Application {
protected:
  App();

public:
  static Glib::RefPtr<App> create();
  void getCss(const std::string &style);

protected:
  void on_activate() override;

  // error for parsing css file
  static void
  on_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section,
                   const Glib::Error &error);

private:
  LumaStart *create_window();
  void on_hide_window(Gtk::Window *window);

  Glib::RefPtr<Gtk::CssProvider> m_refCssProvider;
};
