#pragma once

#include <iosfwd>

#include "src/formats/lst.hpp"

namespace klmth {
  namespace lst {
    std::vector<std::string> parse_file(std::istream& in);
  }
}
