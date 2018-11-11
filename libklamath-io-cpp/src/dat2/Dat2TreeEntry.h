#ifndef PROJECT_DAT2TREEENTRY_H
#define PROJECT_DAT2TREEENTRY_H

#include <cstdint>
#include <string>

typedef struct Dat2TreeEntry {
    std::string filename;
    bool is_compressed;
    uint32_t decompressed_size;
    uint32_t packed_size;
    uint32_t offset;
} Dat2TreeEntry;

#endif //PROJECT_DAT2TREEENTRY_H
