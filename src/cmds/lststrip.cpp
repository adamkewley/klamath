#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>

#include "third_party/CLI11.hpp"

#include "src/formats/lst_reader.hpp"

namespace {
  using namespace klmth;
  
  void handle_output(std::istream& in, std::ostream& out, const std::string& stream_name) {
    out << "[" << stream_name << "]" << std::endl;
    for (const std::string& entry : lst::read_file(in)) {
      out << entry << std::endl;
    }
  }
  
  void print_stream(std::istream& in, std::ostream& out, const std::string& stream_name) {
    try {
      handle_output(in, out, stream_name);
    } catch (const std::exception& ex) {
      throw std::runtime_error(stream_name + ": " + ex.what());
    }
  }
  
  void print_path(const std::string& path, std::ostream& out) {
    std::fstream in{path, std::ios::in | std::ios::binary};
    
    if (!in.good()) {
      std::stringstream err{};
      err << path << ": " << strerror(errno);
      throw std::runtime_error{err.str()};
    }

    print_stream(in, out, path);
  }
}

int cmd_lststrip(int argc, char** argv) {
  CLI::App app{"strip comments from LST file"};
  std::vector<std::string> paths;
  app.add_option("lst_file", paths, "path to LST file. '-' is interpreted as stdin. Supplying no paths will cause application to read LST from stdin.");

  CLI11_PARSE(app, argc, argv);

  try {
    if (paths.empty()) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      for (const auto& path : paths) {
        if (path == "-") {
          print_stream(std::cin, std::cout, "stdin");
        } else {
          print_path(path, std::cout);
        }
      }
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "lststrip: " << ex.what() << std::endl;
    return 1;
  }
}
