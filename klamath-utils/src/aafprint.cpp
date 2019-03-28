#include "src/aafprint.hpp"

#include <iostream>
#include <fstream>

#include "klamath/aaf.hpp"


namespace {
  using namespace klmth;

  char opacity_to_char(uint8_t opacity) {
    switch (opacity) {
    case 0:
      return ' ';
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    default:
      throw std::runtime_error(std::to_string(opacity) + ": unknown opacity value");
    }
  }

  void print_aaf_glyph(const aaf::Glyph& g, std::ostream& out) {
    size_t row_base_idx = 0;

    for (size_t row = 0; row < g.height; ++row) {
      for (size_t col = 0; col < g.width; ++col) {
        uint8_t opacity = g.opacities[row_base_idx + col];
        out << opacity_to_char(opacity);
      }
      out << std::endl;
      row_base_idx += g.width;
    }
  }

  void print_aaf_file(const aaf::File& in, std::ostream& out) {
    for (char c =  97; c < 123; ++c) {
      out << c << ":" << std::endl;
      print_aaf_glyph(in.glyphs[c], out);
      out << "---" << std::endl;
    }
  }
}


int klmth::aaf_print_main(int argc, const char** argv) {
  if (argc == 1) {
    try {
      aaf::File aaf_file = aaf::read_file(std::cin);
      print_aaf_file(aaf_file, std::cout);
    } catch (const std::exception& ex) {
      std::cerr << argv[0] << ": error reading aaf from stdin: " << ex.what() << std::endl;
      return 1;
    }
  } else {
    for (int i = 1; i < argc; ++i) {
      const char* aaf_pth = argv[i];

      std::fstream in;
      in.open(aaf_pth, std::ios::in | std::ios::binary);

      if (!in.good()) {
        std::cerr << argv[0] << ": " << aaf_pth << ": error opening aaf" << std::endl;
        return 1;
      }

      try {
        aaf::File f = aaf::read_file(in);
        print_aaf_file(f, std::cout);
      } catch (const std::exception& ex) {
        std::cerr << argv[0] << ": " << aaf_pth << ": error reading aaf file: " << ex.what() << std::endl;
        return 1;
      }
    }
  }

  return 0;
}
