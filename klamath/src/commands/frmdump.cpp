#include "src/commands/frmdump.hpp"

#include <iostream>
#include <fstream>

#include "vendor/CLI11.hpp"

#include "src/parsers/frm.hpp"

namespace {
  using namespace klmth;

  static const std::string stdin_senteniel{"-"};
  
  frm::OrientableAnimation read_file(std::istream& in, const std::string& in_name) {
    try {
      return frm::read_orientable_anim(in);
    } catch (const std::exception& ex) {
      throw std::runtime_error(in_name + ": error parsing data: " + ex.what());
    }
  }
  
  void print_stream(std::istream& in, std::ostream& out, const std::string& in_name) {
    frm::OrientableAnimation orientable_anim = read_file(in, in_name);

    out << "[" << in_name << "]" << std::endl;
    // todo
    out << std::endl;
  }
  
  void print_path(const std::string& path) {
    if (path == stdin_senteniel) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      std::fstream frm_in;
      frm_in.open(path, std::ios::in | std::ios::binary);

      if (!frm_in.good()) {
        throw std::runtime_error(std::string(path) + ": error when opening frm file");
      }

      print_stream(frm_in, std::cout, path);
    }
  }
}

int klmth::frmdump_main(int argc, char** argv) {
  CLI::App app{"dump FRM data as plaintext"};
  std::vector<std::string> frm_paths;
  app.add_option("frm_file", frm_paths, "path to FRM file(s). '-' is interpreted as stdin. Supplying no paths will cause application to read FRM data from stdin");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& ex) {
    return app.exit(ex);
  }

  try {
    if (frm_paths.empty()) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      for (const std::string& frm_path : frm_paths) {
        print_path(frm_path);
      }
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "frmdump: " << ex.what() << std::endl;
    return 1;
  }
}
