#include "dat2_entry_metadata.h"

namespace klamath {

    Dat2EntryMetadata::Dat2EntryMetadata(
            size_t offset,
            size_t packed_size,
            size_t decompressed_size) :
            _offset(offset),
            _packed_size(packed_size),
            _decompressed_size(decompressed_size) {}

    size_t Dat2EntryMetadata::get_packed_size() const {
        return this->_packed_size;
    }

    size_t Dat2EntryMetadata::get_offset() const {
        return this->_offset;
    }

    size_t Dat2EntryMetadata::get_decompressed_size() const {
        return this->_decompressed_size;
    }
}