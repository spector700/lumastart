#pragma once

#include "glibmm/bytes.h"
#include "glibmm/refptr.h"

namespace Utils {
const Glib::RefPtr<const Glib::Bytes>
getSourceFile(const std::string &filename);

void writeSourceFile(const Glib::RefPtr<const Glib::Bytes> &source_file,
                     const std::string &path);
} // namespace Utils
