#pragma once

#include <iosfwd>
#include <fstream>
#include <string>

namespace klmth {
  namespace cli {
    struct NamedStream {
      std::istream& strm;
      std::string name;
    };

    std::ifstream open_file(const std::string& pth);
  }
}
