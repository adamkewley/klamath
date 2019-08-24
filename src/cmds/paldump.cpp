#include <iostream>
#include <fstream>

#include "third_party/CLI11.hpp"

#include "src/formats/pal_reader.hpp"
#include "src/utils/cli.hpp"

using namespace klmth;

namespace {
  void print_pal(pal::File f, std::ostream& out) {
    out << "palette (rgb):" << std::endl;
    for (const pal::Rgb& rgb : f.palette) {
      out << std::to_string(rgb.r) << " "
          << std::to_string(rgb.g) << " "
          << std::to_string(rgb.b) << std::endl;
    }
  }
}


int cmd_paldump(int argc, char** argv) {
  CLI::App app{"dump pal color indices as plaintext"};
  std::vector<std::string> paths;
  app.add_option("pal_file", paths, "path to PAL file. '-' is interpreted as stdin. Supplying no paths will cause application to read AAF data from stdin");

  CLI11_PARSE(app, argc, argv);

  try {
    auto handler = [](cli::NamedStream& strm) {
                     pal::File f = pal::parse(strm.strm);
                     print_pal(f, std::cout);
                   };
    cli::handle_paths(paths, handler);

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": " << ex.what() << std::endl;
    return 1;
  }
}
