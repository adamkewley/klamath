#pragma once

#include "../utils/Stream.h"
#include "../utils/RandomAccessStream.h"
#include "Dat2TopLevelHeaders.h"
#include "Dat2TreeEntry.h"

namespace klamath {
    const uint8_t TREE_ENT_FILENAME_FIELD_LEN = 4;
    const uint8_t TREE_ENT_IS_COMPRESSED_FIELD_LEN = 1;
    const uint8_t TREE_ENT_DECOMP_SIZE_FIELD_LEN = 4;
    const uint8_t TREE_ENT_PACKED_SIZE_FIELD_LEN = 4;
    const uint8_t TREE_ENT_OFFSET_FIELD_LEN = 4;
    const uint8_t TREE_ENT_FOOTER_FIELD_LEN =
            TREE_ENT_IS_COMPRESSED_FIELD_LEN + TREE_ENT_DECOMP_SIZE_FIELD_LEN + TREE_ENT_PACKED_SIZE_FIELD_LEN + TREE_ENT_OFFSET_FIELD_LEN;
    const uint8_t TREE_ENT_MIN_SIZE =
            TREE_ENT_FILENAME_FIELD_LEN + TREE_ENT_FOOTER_FIELD_LEN;

    Dat2TopLevelHeaders dat2_parse_top_level_headers(RandomAccessStream& s);
    Dat2TreeEntry dat2_parse_tree_entry(Stream& s);
}
