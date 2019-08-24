#include <iostream>
#include <fstream>

#include "third_party/CLI11.hpp"

#include "src/formats/aaf_reader.hpp"
#include "src/utils/cli.hpp"


namespace {
  using namespace klmth;

  char opacity_to_char(uint8_t opacity) {
    constexpr std::array<char, 10> opacities {
      ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    if (opacity >= opacities.size()) {
      throw std::runtime_error(std::to_string(opacity) + ": unknown opacity value");
    }

    return opacities[opacity];
  }

  void print_aaf_glyph(const aaf::Glyph& g, std::ostream& out) {
    size_t row_base_idx = 0;

    for (size_t row = 0; row < g.dimensions.height; ++row) {
      for (size_t col = 0; col < g.dimensions.width; ++col) {
        uint8_t opacity = g.opacities[row_base_idx + col];
        out << opacity_to_char(opacity);
      }
      out << std::endl;
      row_base_idx += g.dimensions.width;
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

int cmd_aafprint(int argc, char** argv) {
  CLI::App app{"print AAF glyphs in plaintext"};
  std::vector<std::string> aaf_pths;
  app.add_option("aaf_file", aaf_pths, "path to AAF file. '-' is interpreted as stdin. Supplying no paths will cause application to read AAF data from stdin");

  CLI11_PARSE(app, argc, argv);

  auto path_handler = [](cli::NamedStream& strm) {
                        aaf::File aaf_file = aaf::read_file(strm.strm);
                        print_aaf_file(aaf_file, std::cout);
                      };

  return cli::main_with_paths("aafprint", aaf_pths, path_handler);
}
