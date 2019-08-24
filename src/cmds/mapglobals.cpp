#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"
#include "src/utils/cli.hpp"

using namespace klmth;

namespace {
  using klmth::map::File;
  
  void print_stream(cli::NamedStream& in, std::ostream& out) {
    out << "[" << in.name << "]" << std::endl;
    File f = map::read_file(in.strm);
    for (int32_t global_var : f.global_vars) {
      out << global_var << std::endl;
    }
  }
}

int cmd_mapglobals(int argc, char** argv) {
  CLI::App app{"dump MAP file globals as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file. '-' is interpreted as stdin. Supplying no paths will cause application to read MAP from stdin");

  CLI11_PARSE(app, argc, argv);

  auto path_handler = [](cli::NamedStream& strm) {
                        print_stream(strm, std::cout);
                      };

  return cli::main_with_paths("mapglobals", paths, path_handler);
}
