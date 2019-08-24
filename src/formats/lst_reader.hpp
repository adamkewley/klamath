#pragma once

#include <iosfwd>

#include "src/formats/lst.hpp"

namespace klmth {
  namespace lst {
    std::vector<std::string> read_file(std::istream& in);
  }
}
