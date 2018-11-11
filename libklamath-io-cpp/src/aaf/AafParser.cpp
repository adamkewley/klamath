#include "AafParser.h"

#include <cstdint>
#include <stdexcept>

#include "../lib/multibyte_readers.h"

namespace {
    const size_t MIN_FILE_SIZE = 0x080c;
    const size_t MAX_GLYPH_HEIGHT_START = 0x4;
    const size_t LETTER_SPACING_START = 0x6;
    const size_t SPACE_WIDTH_START = 0x8;
    const size_t LINE_SPACING_START = 0xa;
    const size_t GLYPH_DESCRIPTIONS_START = 0xc;
    const size_t NUM_GLYPHS_IN_AAF_FILE = 256;
    const size_t GLYPH_DATA_START = 0x080c;


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


    AafHeader read_aaf_header(const uint8_t* data) {
        if (data[0] != 'A' || data[1] != 'A' || data[2] != 'F' || data[3] != 'F') {
            throw std::runtime_error("First four bytes of an AAF file is not the magic number (AAFF)");
        }

        return AafHeader {
            .max_glyph_height = klamath::read_be_u16(data + MAX_GLYPH_HEIGHT_START),
            .letter_spacing = klamath::read_be_u16(data + LETTER_SPACING_START),
            .space_width = klamath::read_be_u16(data + SPACE_WIDTH_START),
            .line_spacing = klamath::read_be_u16(data + LINE_SPACING_START),
        };
    }

    AafGlyphDescription read_glyph_description(const uint8_t* data) {
        AafGlyphDescription desc = {
                .width = klamath::read_be_u16(data),
                .height = klamath::read_be_u16(data + 2),
                .offset_in_glyph_data = klamath::read_be_u32(data + 4),
        };

        return desc;
    }

    std::vector<AafGlyphDescription> read_aaf_glyph_descriptions(const uint8_t* data, size_t size) {
        std::vector<AafGlyphDescription> descriptions;
        descriptions.reserve(sizeof(AafGlyphDescription) * 255);

        const uint8_t* glyph_description = data;
        for (uint32_t i = 0; i < NUM_GLYPHS_IN_AAF_FILE; ++i) {
            descriptions.push_back(read_glyph_description(glyph_description));
            glyph_description += sizeof(AafGlyphDescription);
        }

        return descriptions;
    }

    std::pair<klamath::AafGlyph, size_t> read_glyph(const uint8_t* data, size_t size, const AafGlyphDescription& description) {
        size_t num_bytes_to_read = description.width * description.height;

        if (size < num_bytes_to_read) {
            throw std::runtime_error("Not enough data left in aaf file to read glyph data");
        }

        std::vector<uint8_t> opacities;
        opacities.reserve(num_bytes_to_read);
        for (size_t i = 0; i < num_bytes_to_read; i++) {
            opacities.push_back(data[i]);
        }

        return {
            klamath::AafGlyph {
                .width = description.width,
                .height = description.height,
                .opacities = std::move(opacities)
            },
            num_bytes_to_read
        };
    }
}

namespace klamath {

    AafFile aaf_parse(const uint8_t *data, size_t size) {
        if (size < MIN_FILE_SIZE) {
            throw std::runtime_error("AAF file is below minimum size");
        }

        AafHeader header = read_aaf_header(data);
        std::vector<AafGlyphDescription> descriptions = read_aaf_glyph_descriptions(data + GLYPH_DESCRIPTIONS_START, size - GLYPH_DESCRIPTIONS_START);


        const uint8_t* glyph_data = data + GLYPH_DATA_START;

        std::vector<AafGlyph> glyphs;
        glyphs.reserve(NUM_GLYPHS_IN_AAF_FILE);

        for(const auto& description : descriptions) {
            std::pair<AafGlyph, size_t> p = read_glyph(glyph_data, size, description);

            glyphs.push_back(std::move(p.first));
            glyph_data += p.second;
            size -= p.second;
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
