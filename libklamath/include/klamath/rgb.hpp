#pragma once

namespace klmth {
  struct Rgb {
    Rgb() {
    }
    
    Rgb(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b) {
    }

    Rgb operator*(uint8_t scalar) const {
      return Rgb(r * scalar, g * scalar, b * scalar);
    }
    
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };
}
