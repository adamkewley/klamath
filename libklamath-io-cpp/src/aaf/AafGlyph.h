#ifndef PROJECT_AAFGLYPH_H
#define PROJECT_AAFGLYPH_H

#include <cstdint>
#include <vector>

namespace klamath {
    class AafGlyph {
    public:
        uint16_t get_width() const;
        uint16_t get_height() const;
        uint8_t get_pixel_brightness_at(uint16_t x, uint16_t y) const;

    private:
        uint16_t _width;
        uint16_t _height;
        std::vector<uint8_t> _pixels;
    };
}


#endif //PROJECT_AAFGLYPH_H
