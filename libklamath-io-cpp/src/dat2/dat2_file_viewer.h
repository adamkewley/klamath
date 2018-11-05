#ifndef PROJECT_DAT_READER_H
#define PROJECT_DAT_READER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../lib/optional.h"
#include "../lib/Mmap.h"
#include "dat2_entry_metadata.h"

namespace klamath {

    class Dat2FileViewer {

    public:
        static Dat2FileViewer from_file(const std::string& path);

        Dat2FileViewer(Dat2FileViewer&&) noexcept;

        Dat2FileViewer(const Dat2FileViewer&) = delete;  // We don't want accidental copies

        tl::optional<std::vector<uint8_t>> read_entry(const std::string& path) const;

        std::vector<std::string> entry_names() const;

    private:
        Dat2FileViewer(Mmap mmap, std::unordered_map<std::string, Dat2EntryMetadata> entries);

        Mmap _mmap;
        std::unordered_map<std::string, Dat2EntryMetadata> _entries;
    };
}

#endif //PROJECT_DAT_READER_H
