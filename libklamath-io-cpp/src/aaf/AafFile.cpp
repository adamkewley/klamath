#include "AafFile.h"

namespace klamath {

    AafFile::AafFile(
            uint16_t max_glyph_height,
            uint16_t letter_spacing,
            uint16_t line_spacing,
            uint16_t space_width,
            std::vector<AafGlyph> glyphs) :
            _max_glyph_height(max_glyph_height),
            _letter_spacing(letter_spacing),
            _line_spacing(line_spacing),
            _space_width(space_width),
            _glyphs(std::move(glyphs)) { }

    uint16_t AafFile::get_max_glyph_height() const {
        return this->_max_glyph_height;
    }

    uint16_t AafFile::get_letter_spacing() const {
        return this->_letter_spacing;
    }

    uint16_t AafFile::get_line_spacing() const {
        return this->_line_spacing;
    }

    uint16_t AafFile::get_space_width() const {
        return this->_space_width;
    }

    AafGlyph const &AafFile::get_glyph_for_ascii_char(char c) const {
        return this->_glyphs.at(c);
    }
}
