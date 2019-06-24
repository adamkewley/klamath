#include "src/commands/paldump.hpp"

#include <iostream>
#include <fstream>

#include "third_party/CLI11.hpp"

#include "src/formats/pal_reader.hpp"


namespace {
  using namespace klmth;

  const std::string stdin_senteniel = "-";


  void print_pal(pal::File f, std::ostream& out) {
    out << "palette (rgb):" << std::endl;
    for (const pal::Rgb& rgb : f.palette) {
      out << std::to_string(rgb.r) << " "
          << std::to_string(rgb.g) << " "
          << std::to_string(rgb.b) << std::endl;
    }
  }

  pal::File parse_pal(std::istream& in, std::ostream& out, const std::string& in_name) {
    try {
      return pal::parse(in);
    } catch (const std::exception& ex) {
      throw std::runtime_error(in_name + ": error parsing + printing: " + ex.what());
    }
  }

  void print_stream(std::istream& in, std::ostream& out, const std::string& in_name) {
    pal::File f = parse_pal(in, out, in_name);
    print_pal(f, out);
  }

  void print_path(const std::string& source, std::ostream& out) {
    if (source == stdin_senteniel) {
      print_stream(std::cin, out, "stdin");
    } else {
      std::ifstream in;
      in.open(source, std::ios::in | std::ios::binary);

      if (!in.good()) {
        throw std::runtime_error(source + ": error opening file");
      }

      print_stream(in, out, source);
    }
  }
}


int klmth::paldump_main(int argc, char** argv) {
  CLI::App app{"dump pal color indices as plaintext"};
  std::vector<std::string> pal_pths;
  app.add_option("pal_file", pal_pths, "path to PAL file. '-' is interpreted as stdin. Supplying no paths will cause application to read AAF data from stdin");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& ex) {
    return app.exit(ex);
  }

  try {
    if (pal_pths.empty()) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      for (auto& pal_path : pal_pths) {
        print_path(pal_path, std::cout);
      }
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": " << ex.what() << std::endl;
    return 1;
  }
}
