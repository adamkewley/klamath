#pragma once

#include <cstddef>

namespace klamath {
    const size_t NUM_FRM_ORIENTATIONS = 6;

    enum FrmOrientations {
        NORTH_EAST = 0,
        EAST = 1,
        SOUTH_EAST = 2,
        SOUTH_WEST = 3,
        WEST = 4,
        NORTH_WEST = 5,
    };
}
