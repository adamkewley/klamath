#pragma once

#include <cstdint>
#include <array>
#include <cstdint>
#include <iosfwd>

#include "klamath/rgb.hpp"

namespace klmth {
  namespace pal {
    const unsigned num_palette_colors = 256;
    const unsigned num_conversion_values = 32768;

    struct File {
      std::array<Rgb, num_palette_colors> palette;
      std::array<uint8_t, num_conversion_values> rgb_conversion_table;
    };

    File parse(std::istream& in);
  }
}
