#ifndef PROJECT_AAFGLYPH_H
#define PROJECT_AAFGLYPH_H

#include <cstdint>
#include <vector>

namespace klamath {
    struct AafGlyph {
        uint16_t width;
        uint16_t height;
        std::vector<uint8_t> opacities;
    };
}

#endif //PROJECT_AAFGLYPH_H
