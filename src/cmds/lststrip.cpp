#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>

#include "third_party/CLI11.hpp"

#include "src/formats/lst_reader.hpp"
#include "src/utils/cli.hpp"

using namespace klmth;

namespace {
  void handle_output(cli::NamedStream& in, std::ostream& out) {
    out << "[" << in.name << "]" << std::endl;
    for (const std::string& entry : lst::read_file(in.strm)) {
      out << entry << std::endl;
    }
  }
}

int cmd_lststrip(int argc, char** argv) {
  CLI::App app{"strip comments from LST file"};
  std::vector<std::string> paths;
  app.add_option("lst_file", paths, "path to LST file. '-' is interpreted as stdin. Supplying no paths will cause application to read LST from stdin.");

  CLI11_PARSE(app, argc, argv);

  try {
    auto handler = [](cli::NamedStream& strm) {
                     handle_output(strm, std::cout);
                   };
    cli::handle_paths(paths, handler);
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "lststrip: " << ex.what() << std::endl;
    return 1;
  }
}
