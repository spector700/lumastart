#include "glibmm/refptr.h"
#include "gtkmm/application.h"
#include "gtkmm/window.h"
#include "window.h"

class App : public Gtk::Application {
protected:
  App();

public:
  static Glib::RefPtr<App> create();

protected:
  void on_activate() override;

private:
  LumaStart *create_window();
  void on_hide_window(Gtk::Window *window);
};
