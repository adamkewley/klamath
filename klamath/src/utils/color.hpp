#pragma once

#include <cstdint>

namespace klmth {
  struct Rgb {
    Rgb() noexcept;
    
    Rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept;
    
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
  };

  Rgb scale_brightness(Rgb rgb, uint8_t scalar);
}
