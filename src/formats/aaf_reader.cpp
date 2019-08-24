#include "src/formats/aaf_reader.hpp"

#include <stdexcept>
#include <istream>
#include <cstring>
#include <iostream>

#include "src/utils/io.hpp"

using klmth::read_be_u32;
using klmth::read_be_u16;

namespace {
  constexpr std::array<uint8_t, 4> magic_nums{ 'A', 'A', 'F', 'F' };
  static const unsigned max_glyph_height_len = 2;
  static const unsigned letter_spacing_len = 2;
  static const unsigned space_width_len = 2;
  static const unsigned line_spacing_len = 2;
  static const unsigned header_len =
    max_glyph_height_len + letter_spacing_len + space_width_len + line_spacing_len;

  static const unsigned glyph_width_len = 2;
  static const unsigned glyph_height_len = 2;
  static const unsigned glyph_opacities_offset = 4;
  static const unsigned glyph_min_len =
    glyph_width_len + glyph_height_len + glyph_opacities_offset;  // 0 * 0 dimensions

  static const unsigned min_aaf_len =
    magic_nums.size() + header_len + (aaf::num_glyphs * glyph_min_len);


  void read_header(std::istream& c, aaf::File& out) {
    std::array<uint8_t, magic_nums.size()> mn_data =
      klmth::read<magic_nums.size()>(c);

    if (mn_data != magic_nums) {
      throw std::runtime_error("first four bytes of an AAF file are not the magic number (AAFF)");
    }

    out.max_glyph_height = read_be_u16(c);
    out.letter_spacing = read_be_u16(c);
    out.space_width = read_be_u16(c);
    out.line_spacing = read_be_u16(c);
  }
}

aaf::File aaf::read_file(std::istream& in) {
  aaf::File out;

  read_header(in, out);

  // read glyph headers
  for (aaf::Glyph& g : out.glyphs) {
    g.dimensions.width = read_be_u16(in);
    g.dimensions.height = read_be_u16(in);
    read_be_u32(in);
  }

  // read glyph opacities
  for (aaf::Glyph& g : out.glyphs) {
    size_t num_opacities = g.dimensions.width * g.dimensions.height;
    g.opacities = klmth::read(in, num_opacities);
  }

  return out;
}
