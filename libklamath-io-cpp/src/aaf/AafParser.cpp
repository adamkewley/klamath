#include "AafParser.h"

#include <stdexcept>

namespace {
    using namespace klamath;

    const size_t NUM_GLYPHS_IN_AAF_FILE = 256;


    struct AafHeader {
        uint16_t max_glyph_height;
        uint16_t letter_spacing;
        uint16_t space_width;
        uint16_t line_spacing;
    };

    struct AafGlyphDescription {
        uint16_t width;
        uint16_t height;
        uint32_t offset_in_glyph_data;
    };


    AafHeader read_aaf_header(Stream& s) {
        if (s.read_u8() != 'A' || s.read_u8() != 'A' || s.read_u8() != 'F' || s.read_u8() != 'F') {
            throw std::runtime_error("First four bytes of an AAF file is not the magic number (AAFF)");
        }

        return AafHeader {
            .max_glyph_height = s.read_big_endian_u16(),
            .letter_spacing = s.read_big_endian_u16(),
            .space_width = s.read_big_endian_u16(),
            .line_spacing = s.read_big_endian_u16(),
        };
    }

    AafGlyphDescription read_glyph_description(Stream& s) {
        AafGlyphDescription desc = {
                .width = s.read_big_endian_u16(),
                .height = s.read_big_endian_u16(),
                .offset_in_glyph_data = s.read_big_endian_u32(),
        };

        return desc;
    }

    std::vector<AafGlyphDescription> read_aaf_glyph_descriptions(Stream& s) {
        std::vector<AafGlyphDescription> descriptions;
        descriptions.reserve(sizeof(AafGlyphDescription) * 255);

        for (uint32_t i = 0; i < NUM_GLYPHS_IN_AAF_FILE; ++i) {
            descriptions.push_back(read_glyph_description(s));
        }

        return descriptions;
    }

    AafGlyph read_glyph(Stream& s, const AafGlyphDescription& description) {
        return {
                .width = description.width,
                .height = description.height,
                .opacities = s.read_n_bytes(description.width * description.height),
        };
    }
}

namespace klamath {

    AafFile aaf_parse(Stream &s) {
        AafHeader header = read_aaf_header(s);
        std::vector<AafGlyphDescription> descriptions = read_aaf_glyph_descriptions(s);

        std::vector<AafGlyph> glyphs;
        glyphs.reserve(descriptions.size());
        for(const auto& description : descriptions) {
            glyphs.push_back(read_glyph(s, description));
        }

        return AafFile {
                .max_glyph_height = header.max_glyph_height,
                .letter_spacing = header.letter_spacing,
                .space_width = header.space_width,
                .line_spacing = header.line_spacing,
                .glyphs = std::move(glyphs),
        };
    }
}
