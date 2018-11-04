#include <utility>
#include <sstream>

#include "dat2_file_viewer.h"

namespace {
    const size_t NUM_FILES_FIELD_LEN = 4;
    const size_t TREE_SIZE_FIELD_LEN = 4;
    const size_t DAT2_FILE_SIZE_FIELD_LEN = 4;
    const size_t FOOTER_FIELD_LEN = TREE_SIZE_FIELD_LEN + DAT2_FILE_SIZE_FIELD_LEN;
    const size_t MIN_DAT2_FILE_SIZE = NUM_FILES_FIELD_LEN + FOOTER_FIELD_LEN;

    const size_t TREE_ENT_FILENAME_FIELD_LEN = 4;
    const size_t TREE_ENT_IS_COMPRESSED_FIELD_LEN = 1;
    const size_t TREE_ENT_DECOMP_SIZE_FIELD_LEN = 4;
    const size_t TREE_ENT_PACKED_SIZE_FIELD_LEN = 4;
    const size_t TREE_ENT_OFFSET_FIELD_LEN = 4;
    const size_t TREE_ENT_FOOTER_FIELD_LEN =
            TREE_ENT_IS_COMPRESSED_FIELD_LEN + TREE_ENT_DECOMP_SIZE_FIELD_LEN + TREE_ENT_PACKED_SIZE_FIELD_LEN + TREE_ENT_OFFSET_FIELD_LEN;
    const size_t TREE_ENT_MIN_SIZE =
            TREE_ENT_FILENAME_FIELD_LEN + TREE_ENT_FOOTER_FIELD_LEN;

    typedef struct TopLevelDat2Structure {
        size_t data_section_size;
        size_t num_files;
        size_t tree_offset;
        size_t tree_size;
    } TopLevelDat2Structure;

    typedef struct Dat2TreeEntry {
        std::string filename;
        bool is_compressed;
        size_t decompressed_size;
        size_t packed_size;
        size_t offset;
    } Dat2TreeEntry;

    uint32_t read_little_endian_u32(const void* ptr) {
        return *((uint32_t*)ptr);
    }

    TopLevelDat2Structure parse_top_lvl_structure(const klamath::Mmap& data) {
        size_t size = data.size();
        const uint8_t* data_ptr = data.get();

        if (size < MIN_DAT2_FILE_SIZE) {
            std::stringstream errmsg;
            errmsg << "amount of data available (" << data.size() << " bytes) is too small. Minimum size of DAT2 file is " << MIN_DAT2_FILE_SIZE << " bytes";
            throw std::runtime_error(errmsg.str());
        }

        size_t declared_size = read_little_endian_u32(data_ptr+(size-DAT2_FILE_SIZE_FIELD_LEN));

        if (declared_size != size) {
            std::stringstream errmsg;
            errmsg << "filesize declared in DAT2 footer (" << declared_size << ") does not match actual size of the file (" << size << ")";
            throw std::runtime_error(errmsg.str());
        }

        size_t footer_start = size - FOOTER_FIELD_LEN;

        size_t tree_size = read_little_endian_u32(data_ptr + footer_start);

        if (tree_size > footer_start) {
            std::stringstream errmsg;
            errmsg << "size of the DAT2 file (" << size << ") is too small to even fit the declared tree data (" << tree_size << ")";
            throw std::runtime_error(errmsg.str());
        }

        size_t tree_start = footer_start - tree_size;

        if (tree_size < NUM_FILES_FIELD_LEN) {
            std::stringstream errmsg;
            errmsg << "invalid DAT2 structure. Does not have enough room to fit the number of files. (size = " << size << ", tree start = " << tree_start << ", footer start = " << footer_start << ")";
            throw std::runtime_error(errmsg.str());
        }

        size_t num_files_start = tree_start - NUM_FILES_FIELD_LEN;

        size_t num_files = read_little_endian_u32(data_ptr + num_files_start);

        return TopLevelDat2Structure {
            .data_section_size = num_files_start,
            .num_files = num_files,
            .tree_offset = tree_start,
            .tree_size = footer_start - tree_start,
        };
    }

    std::pair<Dat2TreeEntry, size_t> read_tree_entry(const uint8_t* data, size_t data_remaining) {
        if (data_remaining < TREE_ENT_MIN_SIZE) {
            throw std::runtime_error("invalid DAT2 tree entry: not enough data remaining in DAT2 tree to fit even an empty tree entry");
        }

        size_t filename_len = read_little_endian_u32(data);

        size_t total_len = TREE_ENT_FILENAME_FIELD_LEN + filename_len + TREE_ENT_FOOTER_FIELD_LEN;

        if (data_remaining < total_len) {
            std::stringstream errmsg;
            errmsg << "not enough space remaining in DAT2 tree (" << data_remaining << ") to fit the next tree entry (size = " << total_len << ")";
            throw std::runtime_error(errmsg.str());
        }

        const uint8_t* filename_start = data + TREE_ENT_FILENAME_FIELD_LEN;
        std::string filename;
        filename.reserve(filename_len);

        for (size_t i = 0; i < filename_len; i++) {
            filename[i] = filename_start[i];
        }

        const uint8_t* footer_start = data + TREE_ENT_FILENAME_FIELD_LEN + filename_len;
        bool is_compressed = footer_start[0] != 0;
        size_t decompressed_size = read_little_endian_u32(data + TREE_ENT_IS_COMPRESSED_FIELD_LEN);
        size_t packed_size = read_little_endian_u32(data + TREE_ENT_IS_COMPRESSED_FIELD_LEN + TREE_ENT_DECOMP_SIZE_FIELD_LEN);
        size_t offset = read_little_endian_u32(data + TREE_ENT_IS_COMPRESSED_FIELD_LEN + TREE_ENT_DECOMP_SIZE_FIELD_LEN + TREE_ENT_PACKED_SIZE_FIELD_LEN);

        Dat2TreeEntry left = {
                .filename = std::move(filename),
                .is_compressed = is_compressed,
                .decompressed_size = decompressed_size,
                .packed_size = packed_size,
                .offset = offset,
        };

        return {left, total_len};
    }

    std::unordered_map<std::string, klamath::Dat2EntryMetadata> read_tree_entries(const uint8_t* data, size_t data_remaining, size_t num_files) {
        std::unordered_map<std::string, klamath::Dat2EntryMetadata> ret;

        size_t offset = 0;
        for (size_t i = 0; i < num_files; i++) {
            std::pair<Dat2TreeEntry, size_t> entry_and_size = read_tree_entry(data + offset, data_remaining);

            Dat2TreeEntry entry = entry_and_size.first;
            size_t entry_size = entry_and_size.second;
            std::pair<std::string, klamath::Dat2EntryMetadata> map_entry(std::move(entry.filename), klamath::Dat2EntryMetadata(entry.offset, entry.packed_size, entry.decompressed_size));

            ret.insert(map_entry);
            offset += entry_size;
            data_remaining -= offset;
        }

        return ret;
    }
}

namespace klamath {

    Dat2FileViewer Dat2FileViewer::from_file(const std::string &path) {
        Mmap mmap = Mmap::from_file(path);
        TopLevelDat2Structure top_lvl_structure = parse_top_lvl_structure(mmap);
        std::unordered_map<std::string, klamath::Dat2EntryMetadata> entries = read_tree_entries(mmap.get() + top_lvl_structure.tree_offset, top_lvl_structure.tree_size, top_lvl_structure.num_files);
        return Dat2FileViewer(std::move(mmap), std::move(entries));
    }


    Dat2FileViewer::Dat2FileViewer(Mmap mmap, std::unordered_map<std::string, Dat2EntryMetadata> entries) :
            _mmap(std::move(mmap)),
            _entries(std::move(entries)) {
    }

    Dat2FileViewer::Dat2FileViewer(klamath::Dat2FileViewer && o) noexcept :
            _mmap(std::move(o._mmap)),
            _entries(std::move(o._entries)) {
    }

    tl::optional<std::vector<uint8_t>> Dat2FileViewer::read_entry(const std::string &path) const {
        return tl::optional<std::vector<uint8_t>>(*((uint8_t *)this->_mmap.get()));
    }

    std::vector<std::string> Dat2FileViewer::entry_names() const {
        return std::vector<std::string>();
    }
}
