#include "src/utils/color.hpp"

klmth::Rgb::Rgb() noexcept {
}

klmth::Rgb::Rgb(uint8_t _r, uint8_t _g, uint8_t _b) noexcept :
r{_r}, g{_g}, b{_b} {
              }

klmth::Rgb klmth::scale_brightness(klmth::Rgb rgb, uint8_t scalar) {
  return Rgb(rgb.r * scalar, rgb.g * scalar, rgb.b * scalar);
}
