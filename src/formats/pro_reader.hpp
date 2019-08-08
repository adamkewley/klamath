#pragma once

#include <iosfwd>

#include "src/formats/pro.hpp"

namespace klmth {
  namespace pro {
    Header parse_header(std::istream& in);
  }
}
