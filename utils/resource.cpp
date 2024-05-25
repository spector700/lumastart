#include "resource.h"
#include "giomm/resource.h"
#include "glibmm/bytes.h"
#include "glibmm/refptr.h"
#include <fstream>
#include <ios>
#include <iostream>

Glib::RefPtr<const Glib::Bytes> const
Utils::getSourceFile(const std::string &filename) {
  // Get the conf file from the lumastart resource bundle
  const Glib::RefPtr<const Glib::Bytes> source_file =
      Gio::Resource::lookup_data_global("/lumastart/" + filename);
  return source_file;
}

// write the new file from the default source file after lookup from
// getSourceFile
void Utils::writeSourceFile(const Glib::RefPtr<const Glib::Bytes> &source_file,
                            const std::string &path) {
  // Create and open the new file
  std::ofstream file(path, std::ios::binary | std::ios::out);

  if (!file) {
    std::cerr << "Could not create new file at " << path << '\n';
    return;
  }

  gsize size = 0;
  const gconstpointer data = source_file->get_data(size);

  if (data == nullptr || size == 0) {
    std::cerr << "Failed to get the default source data" << '\n';
    return;
  }

  if (!file.write(reinterpret_cast<const char *>(data),
                  source_file->get_size())) {
    std::cerr << "Failed to write default source data to new file" << '\n';
    file.close();
    return;
  }

  file.close();

  std::cout << "Wrote default config to " << path << '\n';
}
