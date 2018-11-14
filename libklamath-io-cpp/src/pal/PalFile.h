#ifndef PROJECT_PALFILE_H
#define PROJECT_PALFILE_H

#include <vector>
#include "RgbTriple.h"

namespace klamath {
    struct PalFile {
        std::vector<RgbTriple> palette;
        std::vector<uint8_t> rgb_conversion_table;
    };
}

#endif //PROJECT_PALFILE_H
