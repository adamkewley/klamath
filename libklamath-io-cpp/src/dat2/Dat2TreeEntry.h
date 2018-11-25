#pragma once

#include <cstdint>
#include <string>

namespace klamath {
    struct Dat2TreeEntry {
        std::string filename;
        bool is_compressed;
        uint32_t decompressed_size;
        uint32_t packed_size;
        uint32_t offset;
    };
}
