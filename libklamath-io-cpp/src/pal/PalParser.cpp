#include "PalParser.h"

#include <stdexcept>

namespace {
    const size_t NUM_PALETTE_COLORS = 256;
    const size_t NUM_CONVERSION_VALUES = 32768;
}

klamath::PalFile klamath::pal_parse(Stream& s) {
    std::vector<PalRgbTriple> palette;
    palette.reserve(NUM_PALETTE_COLORS);

    for (size_t i = 0; i < NUM_PALETTE_COLORS; i++) {
        PalRgbTriple t = {
                .r = s.read_u8(),
                .g = s.read_u8(),
                .b = s.read_u8(),
        };
        palette.push_back(t);
    }

    return PalFile {
        .palette = std::move(palette),
        .rgb_conversion_table = s.read_n_bytes(NUM_CONVERSION_VALUES),
    };
}
