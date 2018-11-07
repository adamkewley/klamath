#ifndef PROJECT_AAFFILE_H
#define PROJECT_AAFFILE_H

#include <cstdint>
#include <vector>

#include "AafGlyph.h"

namespace klamath {
    class AafFile {
    public:
        AafFile(uint16_t max_glyph_height,
                uint16_t letter_spacing,
                uint16_t line_spacing,
                uint16_t space_width,
                std::vector<AafGlyph> glyphs);

        uint16_t get_max_glyph_height() const;
        uint16_t get_letter_spacing() const;
        uint16_t get_line_spacing() const;
        uint16_t get_space_width() const;
        AafGlyph const & get_glyph_for_ascii_char(char c) const;

    private:
        uint16_t _max_glyph_height;
        uint16_t _letter_spacing;
        uint16_t _line_spacing;
        uint16_t _space_width;
        std::vector<AafGlyph> _glyphs;
    };
}

#endif //PROJECT_AAFFILE_H
