#include "src/utils/color.hpp"


klmth::Rgb klmth::scale_brightness(klmth::Rgb rgb, uint8_t scalar) {
  return Rgb(rgb.r * scalar, rgb.g * scalar, rgb.b * scalar);
}
