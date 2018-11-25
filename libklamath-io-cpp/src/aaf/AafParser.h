#pragma once

#include <cstddef>
#include <cstdint>

#include "utils/Stream.h"
#include "AafFile.h"

namespace klamath {
    const size_t AAF_MIN_FILE_SIZE = 0x080c;

    AafFile aaf_parse(Stream& s);
}
