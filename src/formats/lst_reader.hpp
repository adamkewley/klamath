#pragma once

#include <vector>
#include <string>
#include <iosfwd>

namespace klmth::lst {
  std::vector<std::string> parse_file(std::istream& in);
}
