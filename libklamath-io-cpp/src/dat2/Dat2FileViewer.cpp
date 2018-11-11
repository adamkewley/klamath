#include "Dat2FileViewer.h"

#include <utility>
#include <sstream>

#include "lib/ZlibHelpers.h"
#include "Dat2Parser.h"

namespace klamath {

    Dat2FileViewer Dat2FileViewer::from_file(const std::string &path) {
        Mmap mmap = Mmap::from_file(path);
        Dat2FileMetadata file_metadata = dat2_parse_file(mmap.get(), mmap.size());

        std::unordered_map<std::string, klamath::Dat2EntryMetadata> entries;

        dat2_parse_tree_entries(
                mmap.get() + file_metadata.tree_offset,
                file_metadata.tree_size,
                file_metadata.num_files,
                [&entries](Dat2TreeEntry entry) {
                    Dat2EntryMetadata metadata = {entry.offset, entry.packed_size, entry.decompressed_size};
                    std::pair<std::string, Dat2EntryMetadata> e = {std::move(entry.filename), metadata};
                    entries.insert(std::move(e));
                });

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
        if (this->_entries.count(path) == 0) {
            return tl::nullopt;
        } else {
            const Dat2EntryMetadata& entry = this->_entries.at(path);
            const uint8_t* data_ptr = this->_mmap.get() + entry.offset;

            std::vector<uint8_t> data =
                    ZlibHelpers::inflate_all(data_ptr, entry.packed_size, entry.decompressed_size);

            return {std::move(data)};
        }
    }

    std::vector<std::string> Dat2FileViewer::entry_names() const {
        std::vector<std::string> ret(this->_entries.size());
        for (const auto& e : this->_entries) {
            ret.push_back(e.first);
        }
        return ret;
    }
}
