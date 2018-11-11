#ifndef PROJECT_DAT2TOPLEVELSTRUCTURE_H
#define PROJECT_DAT2TOPLEVELSTRUCTURE_H

#include <cstdint>

namespace klamath {
    struct Dat2FileMetadata {
        uint32_t data_section_size;
        uint32_t num_files;
        uint32_t tree_offset;
        uint32_t tree_size;
    };
}

#endif //PROJECT_DAT2TOPLEVELSTRUCTURE_H
