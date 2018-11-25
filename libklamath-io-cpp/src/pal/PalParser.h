#pragma once

#include <cstddef>

#include "../utils/Stream.h"
#include "PalFile.h"

namespace klamath {
    const size_t MIN_PAL_FILE_SIZE = 0x00008300;  // effectively, up to Additional table #1

    PalFile pal_parse(Stream& s);
}
