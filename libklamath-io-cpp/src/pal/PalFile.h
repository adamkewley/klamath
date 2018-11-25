#pragma once

#include <vector>

#include "PalRgbTriple.h"

namespace klamath {
    struct PalFile {
        std::vector<PalRgbTriple> palette;
        std::vector<uint8_t> rgb_conversion_table;
    };
}
