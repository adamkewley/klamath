#ifndef PROJECT_DAT_ENTRY_METADATA_H
#define PROJECT_DAT_ENTRY_METADATA_H

#include <cstddef>

namespace klamath {
    class Dat2EntryMetadata {
    public:
        Dat2EntryMetadata(size_t offset, size_t packed_size, size_t decompressed_size);

        size_t get_offset() const;

        size_t get_packed_size() const;

        size_t get_decompressed_size() const;

    private:
        const size_t _offset;
        const size_t _packed_size;
        const size_t _decompressed_size;
    };
}

#endif //PROJECT_DAT_ENTRY_METADATA_H
