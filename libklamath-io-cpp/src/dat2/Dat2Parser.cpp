#include "Dat2Parser.h"

#include <stdexcept>
#include <sstream>

namespace {
    const uint8_t NUM_FILES_FIELD_LEN = 4;
    const uint8_t TREE_SIZE_FIELD_LEN = 4;
    const uint8_t DAT2_FILE_SIZE_FIELD_LEN = 4;
    const uint8_t FOOTER_FIELD_LEN = TREE_SIZE_FIELD_LEN + DAT2_FILE_SIZE_FIELD_LEN;
    const uint8_t MIN_DAT2_FILE_SIZE = NUM_FILES_FIELD_LEN + FOOTER_FIELD_LEN;
}

klamath::Dat2TopLevelHeaders klamath::dat2_parse_top_level_headers(klamath::RandomAccessStream &s) {
    if (s.remaining() < MIN_DAT2_FILE_SIZE) {
        std::stringstream errmsg;
        errmsg << "amount of data available (" << s.remaining() << " bytes) is too small. Minimum size of DAT2 file is " << MIN_DAT2_FILE_SIZE << " bytes";
        throw std::runtime_error(errmsg.str());
    }

    size_t size = s.remaining();

    s.seekg(size - DAT2_FILE_SIZE_FIELD_LEN);
    uint32_t declared_size = s.read_little_endian_u32();

    if (declared_size != size) {
        std::stringstream errmsg;
        errmsg << "filesize declared in DAT2 footer (" << declared_size << ") does not match actual size of the file (" << size << ")";
        throw std::runtime_error(errmsg.str());
    }

    s.seekg(size - DAT2_FILE_SIZE_FIELD_LEN - TREE_SIZE_FIELD_LEN);
    uint32_t tree_size = s.read_little_endian_u32();

    if (tree_size < (size - DAT2_FILE_SIZE_FIELD_LEN - NUM_FILES_FIELD_LEN)) {
        std::stringstream errmsg;
        errmsg << "size of the DAT2 file (" << size << ") is too small";
        throw std::runtime_error(errmsg.str());
    }

    uint32_t tree_offset = (uint32_t)size - DAT2_FILE_SIZE_FIELD_LEN - tree_size;
    uint32_t num_files_offset = tree_offset - NUM_FILES_FIELD_LEN;

    s.seekg(num_files_offset);

    return Dat2TopLevelHeaders {
            .data_section_size = num_files_offset,
            .num_files = s.read_little_endian_u32(),
            .tree_offset = tree_offset,
            .tree_size = tree_size - TREE_SIZE_FIELD_LEN,
    };
}

klamath::Dat2TreeEntry klamath::dat2_parse_tree_entry(klamath::Stream &s) {

    uint32_t filename_len = s.read_little_endian_u32();

    std::string filename;
    filename.reserve(filename_len);
    for (uint32_t i = 0; i < filename_len; ++i) {
        filename.push_back(s.read_u8());
    }

    return Dat2TreeEntry {
            .filename = std::move(filename),
            .is_compressed = s.read_u8() != 0,
            .decompressed_size = s.read_little_endian_u32(),
            .packed_size = s.read_little_endian_u32(),
            .offset = s.read_little_endian_u32(),
    };
}
