#include "PalParser.h"

#include <lib/stream.h>
#include <stdexcept>

namespace {
    const size_t MIN_PAL_FILE_SIZE = 0x00008300;  // effectively, up to Additional table #1
    const size_t NUM_PALETTE_COLORS = 256;
    const size_t NUM_CONVERSION_VALUES = 32768;

    klamath::PalFile internal_pal_parse(klamath::Stream& s) {
        if (s.remaining() < MIN_PAL_FILE_SIZE) {
            throw std::runtime_error("pal file is smaller than minimum pal file size");
        }

        std::vector<klamath::RgbTriple> palette;
        palette.reserve(NUM_PALETTE_COLORS);

        for (size_t i = 0; i < NUM_PALETTE_COLORS; i++) {
            klamath::RgbTriple t = {
                    .r = s.read_u8(),
                    .g = s.read_u8(),
                    .b = s.read_u8(),
            };
            palette.push_back(t);
        }

        return klamath::PalFile {
            .palette = std::move(palette),
            .rgb_conversion_table = s.read_n_bytes(NUM_CONVERSION_VALUES),
        };
    }
}

klamath::PalFile klamath::pal_parse(const uint8_t *data, size_t size) {
    Stream s = Stream(data, size);
    return internal_pal_parse(s);
}
