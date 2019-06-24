#pragma once

#include <cstdint>
#include <vector>
#include <array>

#include "src/utils/geometry.hpp"

namespace aaf {
    
  using Dimensions = klmth::geometry::Dimensions<uint16_t>;
    
  struct Glyph {
    Dimensions dimensions;
    std::vector<uint8_t> opacities;
  };

  constexpr size_t num_glyphs = 256;
	
  struct File {
    uint16_t max_glyph_height;
    uint16_t letter_spacing;
    uint16_t space_width;
    uint16_t line_spacing;
    std::array<Glyph, num_glyphs> glyphs;
  };
}
