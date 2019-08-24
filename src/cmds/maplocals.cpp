#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"
#include "src/utils/cli.hpp"

namespace {
  using namespace klmth;
  using klmth::map::File;
  
  void print_stream(cli::NamedStream& strm, std::ostream& out) {
    out << "[" << strm.name << "]" << std::endl;
    File f = map::read_file(strm.strm);
    for (int32_t local_var : f.local_vars) {
      out << local_var << std::endl;
    }
  }
}

int cmd_maplocals(int argc, char** argv) {
  CLI::App app{"dump MAP file local vars as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file. '-' is interpreted as stdin. Supplying no paths will cause application to read MAP from stdin");

  CLI11_PARSE(app, argc, argv);

  try {
    auto handler = [](cli::NamedStream& strm) {
                     print_stream(strm, std::cout);
                   };

    cli::handle_paths(paths, handler);

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "maplocals: " << ex.what() << std::endl;
    return 1;
  }
}
