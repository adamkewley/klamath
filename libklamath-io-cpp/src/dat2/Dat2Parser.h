#ifndef PROJECT_DAT2PARSER_H
#define PROJECT_DAT2PARSER_H

#include <cstdint>
#include <cstddef>
#include <functional>

#include "Dat2TopLevelHeaders.h"
#include "Dat2TreeEntry.h"

namespace klamath {
    Dat2TopLevelHeaders dat2_parse_top_level_headers(const uint8_t *data, uint32_t size);
    void dat2_parse_tree_entries(const uint8_t* tree_entries, uint32_t data_len, uint32_t num_entries, std::function<void(Dat2TreeEntry)> on_entry);
}


#endif //PROJECT_DAT2PARSER_H
