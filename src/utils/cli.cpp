#include "cli.hpp"

#include <sstream>
#include <cstring>

using namespace klmth;

std::ifstream cli::open_file(const std::string& pth) {
  std::ifstream f{pth, std::ios::in | std::ios::binary};

  if (f.good()) {
    f.exceptions(std::ifstream::badbit);
    return f;
  } else {
    std::stringstream err;
    err << pth << ": " << strerror(errno);
    throw std::runtime_error{err.str()};
  }
}
