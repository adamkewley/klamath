#pragma once

#include <cstdint>

namespace klamath {
    struct Dat2TopLevelHeaders {
        uint32_t data_section_size;
        uint32_t num_files;
        uint32_t tree_offset;
        uint32_t tree_size;
    };
}
