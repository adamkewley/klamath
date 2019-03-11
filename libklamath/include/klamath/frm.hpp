#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <iosfwd>

namespace klmth {
  namespace frm {
    
    enum Orientation {
      north_east = 0,
      east = 1,
      south_east = 2,
      south_west = 3,
      west = 4,
      north_west = 5,
      num_orientations = 6,
    };
    
    struct Header {
      uint32_t version_number;
      uint16_t fps;
      uint16_t action_frame;
      uint16_t frames_per_direction;
      std::array<int16_t, num_orientations> pixel_shifts_x;
      std::array<int16_t, num_orientations> pixel_shifts_y;
      std::array<uint32_t, num_orientations> offsets_in_frame_data;
      uint32_t size_of_frame_data;
    };

    struct Frame {
      uint16_t width;
      uint16_t height;
      uint32_t size;
      int16_t pixel_shift_x;
      int16_t pixel_shift_y;
      std::vector<uint8_t> color_indices;
    };

    void parse_header(const uint8_t* buf, size_t n, Header& out);
    Header parse_header(std::istream& in);
    void parse_frame(const uint8_t* buf, size_t n, Frame& out);
  }
}
