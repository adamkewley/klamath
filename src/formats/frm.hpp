#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "src/utils/geometry.hpp"

namespace frm {
    
  enum Orientation {
    north_east,
    east,
    south_east,
    south_west,
    west,
    north_west,
    num_orientations,
  };

  constexpr std::array<Orientation, num_orientations> orientations {
    north_east,
    east,
    south_east,
    south_west,
    west,
    north_west,
  };

  using PixelShift = klmth::geometry::Point<int16_t>;
    
  struct Header {    
    uint32_t version_number;
    uint16_t fps;
    uint16_t action_frame;
    uint16_t frames_per_direction;
    std::array<PixelShift, num_orientations> pixel_shifts;
    std::array<uint32_t, num_orientations> offsets_in_frame_data;
    uint32_t size_of_frame_data;
  };

  using Dimensions = klmth::geometry::Dimensions<uint16_t>;

  class Frame {
  public:
    Frame(Dimensions dimensions,
          PixelShift pixel_shift,
          std::vector<uint8_t> data);
    
    Dimensions dimensions() const noexcept;
    PixelShift pixel_shift() const noexcept;
    const std::vector<uint8_t>& data() const noexcept;
    
  private:
    Dimensions _dimensions;
    PixelShift _pixel_shift;
    std::vector<uint8_t> _data;
  };

  class Animation {
  public:
    Animation(uint16_t fps,
              uint16_t action_frame,
              std::vector<Frame> frames);

    Dimensions dimensions() const noexcept;
    uint16_t fps() const noexcept;
    uint16_t action_frame() const noexcept;
    const std::vector<Frame>& frames() const noexcept;
    
  private:
    uint16_t _fps;
    uint16_t _action_frame;
    std::vector<Frame> _frames;
  };

  class File {
  public:
    File(std::vector<Animation> animations,
         std::array<uint8_t, num_orientations> animation_idxs);

    const Animation& animation(Orientation o) const noexcept;
    
  private:
    std::vector<Animation> _animations;
    std::array<uint8_t, num_orientations> _animation_idxs;
  };
}
