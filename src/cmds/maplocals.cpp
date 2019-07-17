#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"

namespace {
  using namespace klmth;
  using klmth::map::File;
  
  void print_stream(std::istream& in, std::ostream& out, const std::string& stream_name) {
    out << "[" << stream_name << "]" << std::endl;
    File f = map::parse_file(in);
    for (int32_t local_var : f.local_vars) {
      out << local_var << std::endl;
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

int cmd_maplocals(int argc, char** argv) {
  CLI::App app{"dump MAP file local vars as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file. '-' is interpreted as stdin. Supplying no paths will cause application to read MAP from stdin");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& ex) {
    return app.exit(ex);
  }

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
    std::cerr << "maplocals: " << ex.what() << std::endl;
    return 1;
  }
}
