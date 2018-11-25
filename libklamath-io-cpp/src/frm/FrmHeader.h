#pragma once

#include <cstdint>
#include <array>

#include "FrmOrientations.h"

namespace klamath {
    struct FrmHeader {
        uint32_t version_number;
        uint16_t fps;
        uint16_t action_frame;
        uint16_t frames_per_direction;
        std::array<int16_t, NUM_FRM_ORIENTATIONS> pixel_shifts_x;
        std::array<int16_t , NUM_FRM_ORIENTATIONS> pixel_shifts_y;
        std::array<uint32_t, NUM_FRM_ORIENTATIONS> offsets_in_frame_data;
        uint32_t size_of_frame_data;
    };
}
