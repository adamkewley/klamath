#pragma once

#include <iosfwd>

#include "src/formats/frm.hpp"

namespace frm {
  Header read_header(std::istream& in);
  File read_file(std::istream& in);
}
