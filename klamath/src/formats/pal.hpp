#pragma once

#include <cstdint>
#include <array>

#include "src/utils/color.hpp"

namespace pal {
  constexpr unsigned num_palette_colors = 256;
  constexpr unsigned num_conversion_values = 32768;

  struct File {
    std::array<klmth::Rgb, num_palette_colors> palette;
    std::array<uint8_t, num_conversion_values> rgb_conversion_table;
  };
}
