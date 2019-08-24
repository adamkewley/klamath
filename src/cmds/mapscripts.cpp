#include "third_party/CLI11.hpp"

#include "src/utils/cli.hpp"
#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"

using namespace klmth;
using klmth::cli::NamedStream;
using klmth::cli::open_file;

namespace {
  void print_stream(NamedStream& strm, std::ostream& out) {
    map::File f = map::read_file(strm.strm);
    
    out << "[" << strm.name << "]" << std::endl;
    for (const map::Script& script : f.scripts) {
      out << "script_pid_type = " << str(script.pid.type()) << std::endl;
    }
  }
}

int cmd_mapscripts(int argc, char** argv) {
  CLI::App app{"dump MAP file scripts as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file");

  CLI11_PARSE(app, argc, argv);

  try {
    if (paths.empty()) {
      NamedStream strm{std::cin, "stdin"};
      print_stream(strm, std::cout);
    } else {
      for (std::string path : paths) {
        if (path == "-") {
          NamedStream strm{std::cin, "stdin"};
          print_stream(strm, std::cout);
        } else {
          std::ifstream fd = open_file(path);
          NamedStream strm{fd, path};
          print_stream(strm, std::cout);
        }
      }
    }
  } catch (const std::exception& ex) {
    std::cerr << "mapscripts: " << ex.what() << std::endl;
    return 1;
  }
  
  return 0;
}
