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

    std::ifstream open_file(const std::string& pth);
    void handle_paths(const std::vector<std::string>& pths, std::function<void(NamedStream&)> callback);
  }
}
