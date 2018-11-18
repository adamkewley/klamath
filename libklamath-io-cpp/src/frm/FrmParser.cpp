#include <lib/stream.h>
#include "FrmParser.h"

namespace {
    using namespace klamath;

    FrmHeader frm_parse_header_internal(Stream& s) {
        uint32_t version_number = s.read_big_endian_u32();
        uint16_t fps = s.read_big_endian_u16();
        uint16_t action_frame = s.read_big_endian_u16();
        uint16_t frames_per_direction = s.read_big_endian_u16();

        std::array<int16_t, NUM_FRM_ORIENTATIONS> pixel_shifts_x {};
        for (int16_t& pixel_shift : pixel_shifts_x) {
            pixel_shift = s.read_big_endian_i16();
        }

        std::array<int16_t, NUM_FRM_ORIENTATIONS> pixel_shifts_y {};
        for (int16_t& pixel_shift : pixel_shifts_y) {
            pixel_shift = s.read_big_endian_i16();
        }

        std::array<uint32_t, NUM_FRM_ORIENTATIONS> offsets_in_frame_data {};
        for (uint32_t& offset : offsets_in_frame_data) {
            offset = s.read_big_endian_u32();
        }

        uint32_t size_of_frame_data = s.read_big_endian_u32();

        return FrmHeader {
            .version_number = version_number,
            .fps = fps,
            .action_frame = action_frame,
            .frames_per_direction = frames_per_direction,
            .pixel_shifts_x = pixel_shifts_x,
            .pixel_shifts_y = pixel_shifts_y,
            .offsets_in_frame_data = offsets_in_frame_data,
            .size_of_frame_data = size_of_frame_data,
        };
    }

    FrmFrame frm_parse_frame_internal(Stream& s) {
        uint16_t width = s.read_big_endian_u16();
        uint16_t height = s.read_big_endian_u16();
        uint32_t size = s.read_big_endian_u32();
        int16_t pixel_shift_x = s.read_big_endian_i16();
        int16_t pixel_shift_y = s.read_big_endian_i16();

        std::vector<uint8_t> color_indices;
        color_indices.reserve(size);
        for (uint32_t i = 0; i < size; i++) {
            color_indices.push_back(s.read_u8());
        }

        return FrmFrame {
            .width = width,
            .height = height,
            .size = size,
            .pixel_shift_x = pixel_shift_x,
            .pixel_shift_y = pixel_shift_y,
            .color_indices = std::move(color_indices),
        };
    }
}

FrmHeader klamath::frm_parse_header(const uint8_t *data, size_t size) {
    Stream s(data, size);
    return frm_parse_header_internal(s);
}

FrmFrame klamath::frm_parse_frame(const uint8_t *data, size_t size) {
    Stream s(data, size);
    return frm_parse_frame_internal(s);
}
