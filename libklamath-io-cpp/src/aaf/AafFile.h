#pragma once

#include <cstdint>
#include <vector>

#include "AafGlyph.h"

namespace klamath {
    struct AafFile {
        uint16_t max_glyph_height;
        uint16_t letter_spacing;
        uint16_t space_width;
        uint16_t line_spacing;
        std::vector<AafGlyph> glyphs;
    };
}
