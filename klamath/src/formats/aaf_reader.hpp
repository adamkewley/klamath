#pragma once

#include <iosfwd>

#include "src/formats/aaf.hpp"

namespace aaf {
  File read_file(std::istream& in);
}
