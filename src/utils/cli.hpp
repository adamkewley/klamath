#pragma once

#include <iosfwd>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

namespace klmth {
  namespace cli {
    struct NamedStream {
      std::istream& strm;
      std::string name;
    };

    int main_with_paths(const char* appname,
                        const std::vector<std::string>& pths,
                        std::function<void(NamedStream&)> callback);
  }
}
