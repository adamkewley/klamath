#include "Dat2Parser.h"

#include <stdexcept>
#include <sstream>

#include "../lib/multibyte_readers.h"

namespace {
    const uint8_t NUM_FILES_FIELD_LEN = 4;
    const uint8_t TREE_SIZE_FIELD_LEN = 4;
    const uint8_t DAT2_FILE_SIZE_FIELD_LEN = 4;
    const uint8_t FOOTER_FIELD_LEN = TREE_SIZE_FIELD_LEN + DAT2_FILE_SIZE_FIELD_LEN;
    const uint8_t MIN_DAT2_FILE_SIZE = NUM_FILES_FIELD_LEN + FOOTER_FIELD_LEN;

    const uint8_t TREE_ENT_FILENAME_FIELD_LEN = 4;
    const uint8_t TREE_ENT_IS_COMPRESSED_FIELD_LEN = 1;
    const uint8_t TREE_ENT_DECOMP_SIZE_FIELD_LEN = 4;
    const uint8_t TREE_ENT_PACKED_SIZE_FIELD_LEN = 4;
    const uint8_t TREE_ENT_OFFSET_FIELD_LEN = 4;
    const uint8_t TREE_ENT_FOOTER_FIELD_LEN =
            TREE_ENT_IS_COMPRESSED_FIELD_LEN + TREE_ENT_DECOMP_SIZE_FIELD_LEN + TREE_ENT_PACKED_SIZE_FIELD_LEN + TREE_ENT_OFFSET_FIELD_LEN;
    const uint8_t TREE_ENT_MIN_SIZE =
            TREE_ENT_FILENAME_FIELD_LEN + TREE_ENT_FOOTER_FIELD_LEN;

    uint32_t read_tree_entry(const uint8_t* data, size_t data_remaining, const std::function<void(Dat2TreeEntry)>& on_entry) {
        if (data_remaining < TREE_ENT_MIN_SIZE) {
            throw std::runtime_error("invalid DAT2 tree entry: not enough data remaining in DAT2 tree to fit even an empty tree entry");
        }

        uint32_t filename_len = klamath::read_le_u32(data);

        uint32_t total_len = TREE_ENT_FILENAME_FIELD_LEN + filename_len + TREE_ENT_FOOTER_FIELD_LEN;

        if (data_remaining < total_len) {
            std::stringstream errmsg;
            errmsg << "not enough space remaining in DAT2 tree (" << data_remaining << ") to fit the next tree entry (size = " << total_len << ")";
            throw std::runtime_error(errmsg.str());
        }

        const uint8_t* filename_start = data + TREE_ENT_FILENAME_FIELD_LEN;
        std::string filename;
        filename.reserve(filename_len);

        for (uint32_t i = 0; i < filename_len; i++) {
            filename.push_back(filename_start[i]);
        }

        const uint8_t* footer_start = data + TREE_ENT_FILENAME_FIELD_LEN + filename_len;
        bool is_compressed = footer_start[0] != 0;
        uint32_t decompressed_size = klamath::read_le_u32(footer_start + TREE_ENT_IS_COMPRESSED_FIELD_LEN);
        uint32_t packed_size = klamath::read_le_u32(
                footer_start + TREE_ENT_IS_COMPRESSED_FIELD_LEN + TREE_ENT_DECOMP_SIZE_FIELD_LEN);
        uint32_t offset = klamath::read_le_u32(
                footer_start + TREE_ENT_IS_COMPRESSED_FIELD_LEN + TREE_ENT_DECOMP_SIZE_FIELD_LEN +
                TREE_ENT_PACKED_SIZE_FIELD_LEN);

        Dat2TreeEntry left = {
                .filename = std::move(filename),
                .is_compressed = is_compressed,
                .decompressed_size = decompressed_size,
                .packed_size = packed_size,
                .offset = offset,
        };

        on_entry(std::move(left));

        return total_len;
    }
}

namespace klamath {
    Dat2FileMetadata dat2_parse_file(const uint8_t* data, uint32_t size) {
        if (size < MIN_DAT2_FILE_SIZE) {
            std::stringstream errmsg;
            errmsg << "amount of data available (" << size << " bytes) is too small. Minimum size of DAT2 file is " << MIN_DAT2_FILE_SIZE << " bytes";
            throw std::runtime_error(errmsg.str());
        }

        uint32_t declared_size = read_le_u32(data + (size - DAT2_FILE_SIZE_FIELD_LEN));

        if (declared_size != size) {
            std::stringstream errmsg;
            errmsg << "filesize declared in DAT2 footer (" << declared_size << ") does not match actual size of the file (" << size << ")";
            throw std::runtime_error(errmsg.str());
        }

        uint32_t footer_start = size - FOOTER_FIELD_LEN;

        uint32_t tree_size = read_le_u32(data + footer_start);

        if (tree_size > footer_start) {
            std::stringstream errmsg;
            errmsg << "size of the DAT2 file (" << size << ") is too small to even fit the declared tree data (" << tree_size << ")";
            throw std::runtime_error(errmsg.str());
        }

        uint32_t tree_start = (footer_start + TREE_SIZE_FIELD_LEN) - tree_size;

        if (tree_size < NUM_FILES_FIELD_LEN) {
            std::stringstream errmsg;
            errmsg << "invalid DAT2 structure. Does not have enough room to fit the number of files. (size = " << size << ", tree start = " << tree_start << ", footer start = " << footer_start << ")";
            throw std::runtime_error(errmsg.str());
        }

        uint32_t num_files_start = tree_start - NUM_FILES_FIELD_LEN;

        uint32_t num_files = read_le_u32(data + num_files_start);

        return Dat2FileMetadata {
                .data_section_size = num_files_start,
                .num_files = num_files,
                .tree_offset = tree_start,
                .tree_size = footer_start - tree_start,
        };
    }

    void dat2_parse_tree_entries(
            const uint8_t* tree_entries,
            uint32_t data_len,
            uint32_t num_entries,
            std::function<void(Dat2TreeEntry)> on_entry) {

        for (size_t i = 0; i < num_entries; i++) {
            uint32_t entry_size = read_tree_entry(tree_entries, data_len, on_entry);
            tree_entries += entry_size;
            data_len -= entry_size;
        }
    }
}
