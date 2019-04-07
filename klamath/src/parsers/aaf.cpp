#include "src/parsers/aaf.hpp"

#include <stdexcept>
#include <istream>

#include "src/utils/cursor.hpp"

using klmth::read_be_u32_unsafe;
using klmth::read_be_u16_unsafe;

namespace {
  const unsigned num_magic_nums = 4;
  const unsigned max_glyph_height_len = 2;
  const unsigned letter_spacing_len = 2;
  const unsigned space_width_len = 2;
  const unsigned line_spacing_len = 2;
  const unsigned header_len =
    max_glyph_height_len + letter_spacing_len + space_width_len + line_spacing_len;

  const unsigned glyph_width_len = 2;
  const unsigned glyph_height_len = 2;
  const unsigned glyph_opacities_offset = 4;
  const unsigned glyph_min_len =
    glyph_width_len + glyph_height_len + glyph_opacities_offset;  // 0 * 0 dimensions

  const unsigned min_aaf_len =
    num_magic_nums + header_len + (klmth::aaf::num_glyphs * glyph_min_len);


  void read_header(klmth::Cursor& c, klmth::aaf::File& out) {
    if (c.remaining() < min_aaf_len) {
      throw std::runtime_error("too little data for an AAF file");
    }

    if (read_u8_unsafe(c) != 'A' || read_u8_unsafe(c) != 'A' || read_u8_unsafe(c) != 'F' || read_u8_unsafe(c) != 'F') {
      throw std::runtime_error("first four bytes of an AAF file are not the magic number (AAFF)");
    }

    out.max_glyph_height = read_be_u16_unsafe(c);
    out.letter_spacing = read_be_u16_unsafe(c);
    out.space_width = read_be_u16_unsafe(c);
    out.line_spacing = read_be_u16_unsafe(c);
  }

  klmth::aaf::File read(const uint8_t* buf, size_t n) {
    klmth::Cursor c{buf, n};
    klmth::aaf::File out;
    
    read_header(c, out);

    // Read glyph headers
    for (size_t i = 0; i < out.glyphs.size(); ++i) {
      if (c.remaining() < glyph_min_len) {
        throw std::runtime_error("ran out of data when reading an aaf glyph header");
      }

      klmth::aaf::Glyph& g = out.glyphs[i];
      g.width = read_be_u16_unsafe(c);
      g.height = read_be_u16_unsafe(c);
      // glyph offset: not needed because the entire file is parsed
      read_be_u32_unsafe(c);
    }

    // Read opacities
    for (klmth::aaf::Glyph& g : out.glyphs) {
      size_t num_opacities = g.width * g.height;

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

klmth::aaf::File klmth::aaf::read_file(std::istream& in) {
  static const size_t max_aaf_size = 1 << 16;

  std::array<uint8_t, max_aaf_size> buf;
  in.read(reinterpret_cast<char*>(buf.data()), buf.size());

  if (in.gcount() == max_aaf_size && in.eof()) {
    throw std::runtime_error("input data too big for an aaf file");
  } else {
    return ::read(buf.data(), in.gcount());
  }
}
