#pragma once

#include <cstdint>

namespace klmth {
  struct Rgb {
    Rgb() noexcept : r(0), g(0), b(0) {
    }
    
    Rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept : r(_r), g(_g), b(_b) {
    }
    
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };

  Rgb scale_brightness(Rgb rgb, uint8_t scalar);
}
