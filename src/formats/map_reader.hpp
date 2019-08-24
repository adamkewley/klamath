#pragma once

#include <iosfwd>

#include "src/formats/map.hpp"

namespace klmth {
  namespace map {
    Header read_header(std::istream& in);
    File read_file(std::istream& in);
  }
}
