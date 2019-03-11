#pragma once

#include <iosfwd>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>

namespace klmth {
  namespace aaf {
    const size_t num_glyphs = 256;
    
    struct Glyph {
      uint16_t width;
      uint16_t height;
      std::vector<uint8_t> opacities;
    };
	
    struct File {
      uint16_t max_glyph_height;
      uint16_t letter_spacing;
      uint16_t space_width;
      uint16_t line_spacing;
      std::array<Glyph, num_glyphs> glyphs;
    };
	
    void read(const uint8_t* buf, size_t n, aaf::File& out);
  }
}
