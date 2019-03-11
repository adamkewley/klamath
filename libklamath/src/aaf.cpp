#include "klamath/aaf.hpp"

#include <stdexcept>

#include "ioutils.hpp"


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
  
  
  void read_header(const uint8_t* buf, size_t n, klmth::aaf::File& out, size_t& offset) {
    if (n < min_aaf_len) {
      throw std::runtime_error("too little data for an AAF file");
    }
    
    if (buf[offset++] != 'A' || buf[offset++] != 'A' || buf[offset++] != 'F' || buf[offset++] != 'F') {
      throw std::runtime_error("first four bytes of an AAF file are not the magic number (AAFF)");
    }
  
    out.max_glyph_height = klmth::read_be_u16(buf, offset);
    out.letter_spacing = klmth::read_be_u16(buf, offset);
    out.space_width = klmth::read_be_u16(buf, offset);
    out.line_spacing = klmth::read_be_u16(buf, offset);
  }
}


void klmth::aaf::read(const uint8_t* buf, size_t n, aaf::File& out) {  
  size_t offset = 0;
  read_header(buf, n, out, offset);

  // Read glyph headers
  for (size_t i = 0; i < out.glyphs.size(); ++i) {
    if (n < (offset + glyph_min_len)) {
      throw std::runtime_error("ran out of data when reading an aaf glyph header");
    }
    
    Glyph& g = out.glyphs[i];
    g.width = klmth::read_be_u16(buf, offset);
    g.height = klmth::read_be_u16(buf, offset);
    // glyph offset: not needed because the entire file is parsed
    klmth::read_be_u32(buf, offset); 
  }

  // Read opacities
  for (Glyph& g : out.glyphs) {
    size_t num_opacities = g.width * g.height;
    
    if (n < (offset + num_opacities)) {
      throw std::runtime_error("ran out of data when reading aaf opacities");
    }
    
    g.opacities.resize(num_opacities);
    g.opacities.insert(g.opacities.begin(), buf + offset, buf + offset + num_opacities);
    offset += num_opacities;
  }
}
