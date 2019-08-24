#include "src/formats/aaf_reader.hpp"

#include <stdexcept>
#include <istream>
#include <cstring>
#include <iostream>

#include "src/utils/io.hpp"

using klmth::read_be_u32;
using klmth::read_be_u16;

namespace {
  static const uint8_t magic_nums[] = { 'A', 'A', 'F', 'F' };
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
    sizeof(magic_nums) + header_len + (aaf::num_glyphs * glyph_min_len);


  void read_header(klmth::Cursor& c, aaf::File& out) {
    if (c.remaining() < min_aaf_len) {
      throw std::runtime_error("too little data for an AAF file");
    }

    if (std::memcmp(c.read_then_advance_unsafe(sizeof(magic_nums)),
                    magic_nums,
                    sizeof(magic_nums)) != 0) {
      throw std::runtime_error("first four bytes of an AAF file are not the magic number (AAFF)");
    }

    out.max_glyph_height = read_be_u16(c);
    out.letter_spacing = read_be_u16(c);
    out.space_width = read_be_u16(c);
    out.line_spacing = read_be_u16(c);
  }

  aaf::File read(const uint8_t* buf, size_t n) {    
    klmth::Cursor c{buf, n};
    aaf::File out;
    
    read_header(c, out);

    // read glyph headers
    for (aaf::Glyph& g : out.glyphs) {
      if (c.remaining() < glyph_min_len) {
        throw std::runtime_error("ran out of data when reading an aaf glyph header");
      }

      g.dimensions.width = read_be_u16(c);
      g.dimensions.height = read_be_u16(c);
      read_be_u32(c);
    }

    // read glyph opacities
    for (aaf::Glyph& g : out.glyphs) {
      size_t num_opacities = g.dimensions.width * g.dimensions.height;

      if (c.remaining() < num_opacities) {
        throw std::runtime_error("ran out of data when reading aaf opacities");
      }

      g.opacities.resize(num_opacities);
      g.opacities.insert(g.opacities.begin(), c.data(), c.data() + num_opacities);
      c.advance_unsafe(num_opacities);
    }

    return out;
  }
}

aaf::File aaf::read_file(std::istream& in) {
  static const size_t max_aaf_size = 1 << 16;

  std::array<uint8_t, max_aaf_size> buf;
  in.read(reinterpret_cast<char*>(buf.data()), buf.size());

  if (in.gcount() == max_aaf_size && in.eof()) {
    throw std::runtime_error("input data too big for an aaf file");
  } else {
    return ::read(buf.data(), in.gcount());
  }
}

