#include "cli.hpp"

#include <sstream>
#include <cstring>
#include <iostream>

using namespace klmth;

namespace {
  void handle(cli::NamedStream& strm,
              std::function<void(cli::NamedStream&)> callback) {
    try {
      callback(strm);
    } catch (const std::exception& ex) {
      std::stringstream msg;
      msg << strm.name << ": " << ex.what();
      throw std::runtime_error{msg.str()};
    }
  }
}

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

void cli::handle_paths(const std::vector<std::string>& pths,
                       std::function<void(NamedStream&)> callback) {
  if (pths.size() == 0) {
    NamedStream ns{std::cin, "stdin"};
    handle(ns, callback);
  } else {
    for (const std::string& pth : pths) {
      if (pth == "-") {
        NamedStream ns{std::cin, "-"};
        handle(ns, callback);
      } else {
        std::ifstream fd = open_file(pth);
        NamedStream ns{fd, pth};
        handle(ns, callback);
      }
    }
  }
}
