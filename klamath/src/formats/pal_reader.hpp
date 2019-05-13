#pragma once

#include <iosfwd>

#include "src/formats/pal.hpp"

namespace pal {
  File parse(std::istream& in);
}
