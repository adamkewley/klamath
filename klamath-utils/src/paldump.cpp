#include "src/paldump.hpp"

#include <iostream>
#include <fstream>

#include "klamath/pal.hpp"


namespace {
  using namespace klmth;

  const std::string stdin_senteniel = "-";


  void print_pal(pal::File f, std::ostream& out) {
    out << "palette (rgb):" << std::endl;
    for (const klmth::Rgb& rgb : f.palette) {
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


int klmth::paldump_main(int argc, const char** argv) {
  if (argc == 0) {
    std::cerr << "fatal error: no application name argument" << std::endl;
    return 1;
  }

  try {
    if (argc == 1) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      for (int i = 1; i < argc; ++i) {
        print_path(argv[i], std::cout);
      }
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": " << ex.what() << std::endl;
    return 1;
  }
}
