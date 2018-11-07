#include <cstddef>
#include "AafGlyph.h"

namespace klamath {
    uint16_t AafGlyph::get_width() const {
        return this->_width;
    }

    uint16_t AafGlyph::get_height() const {
        return this->_height;
    }

    uint8_t AafGlyph::get_pixel_brightness_at(uint16_t x, uint16_t y) const {
        size_t offset = x + (y * _width);
        return this->_pixels.at(offset);
    }
}
