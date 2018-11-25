#pragma once

#include <cstdint>
#include <vector>

namespace klamath {
    struct AafGlyph {
        uint16_t width;
        uint16_t height;
        std::vector<uint8_t> opacities;
    };
}
