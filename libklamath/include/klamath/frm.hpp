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

    Header read_header(std::istream& in);
    

    struct Frame {
      uint16_t width;
      uint16_t height;
      uint32_t size;
      int16_t pixel_shift_x;
      int16_t pixel_shift_y;
      std::vector<uint8_t> color_indices;
    };

    Frame read_frame(std::istream& in);
    
    /*
    struct PixelShift {
      int16_t x;
      int16_t y;
    };

    enum class Type {
      image;
      animated_image;
      orientable;
      animated_orientable;
    };

    class Image {
      PixelShift pixel_shift;
      frm::Frame frame;
    };
    
    class AnimatedImage {
      uint16_t fps;
      uint16_t action_frame;
      PixelShift pixel_shift;
      std::vector<frm::Frame> frames;
    };

    class Orientable {
      
      std::array<PixelShift, num_orientations> _pixel_shifts;
      std::array<Frame, num_orientations> _frames;
    };

    class AnimatedOrientable {
    };
    

    class File {
    public:
      uint16_t fps;
      uint16_t action_frame;
      
      PixelShift pixel_shift(Orientation o) const noexcept;
      const std::vector<frm::Frame>& frames(Orientation o) const noexcept;
    private:
      std::array<PixelShift, num_orientations> _pixel_shifts;
      std::array<uint8_t, num_orientations> _frame_data_idxs;
      std::array<std::unique_ptr<std::vector<Frame>>, num_orientations> _frame_data;
    };
    
    File read_file(std::istream& in);
    */
  }
}
