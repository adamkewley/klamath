#ifndef PROJECT_FRMFRAME_H
#define PROJECT_FRMFRAME_H

#include <cstdint>
#include <vector>

namespace klamath {
    struct FrmFrame {
        uint16_t width;
        uint16_t height;
        uint32_t size;
        int16_t pixel_shift_x;
        int16_t pixel_shift_y;
        std::vector<uint8_t> color_indices;
    };
}

#endif //PROJECT_FRMFRAME_H
