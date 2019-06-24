#pragma once

#include <iosfwd>

#include "src/formats/dat2.hpp"

namespace dat2 {
  Sections read_sections(std::istream& dat2_in);
  TreeEntry read_tree_entry(std::istream& entry_in);
}
