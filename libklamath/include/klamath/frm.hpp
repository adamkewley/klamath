#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <iosfwd>

#include "klamath/geometry.hpp"


namespace klmth {
  namespace frm {
    
    enum Orientation : unsigned char {
      north_east,
      east,
      south_east,
      south_west,
      west,
      north_west,
      num_orientations,
    };

    static const std::array<Orientation, num_orientations> orientations {
      north_east,
      east,
      south_east,
      south_west,
      west,
      north_west,
    };

    using PixelShift = geometry::Point<int16_t>;
    
    struct Header {
      uint32_t version_number;
      uint16_t fps;
      uint16_t action_frame;
      uint16_t frames_per_direction;
      std::array<PixelShift, num_orientations> pixel_shifts;
      std::array<uint32_t, num_orientations> offsets_in_frame_data;
      uint32_t size_of_frame_data;
    };

    using Dimensions = geometry::Dimensions<uint16_t>;
    
    class FrameHeader {
    public:
      FrameHeader(Dimensions dimensions,
                  PixelShift pixel_shift) noexcept :
        _dimensions(std::move(dimensions)),
        _pixel_shift(std::move(pixel_shift)) {
      }
      
      Dimensions dimensions() const noexcept {
        return this->_dimensions;
      }

      PixelShift pixel_shift() const noexcept {
        return this->_pixel_shift;
      }

      void apply_pixel_shift(PixelShift pixel_shift) noexcept {
        _pixel_shift += pixel_shift;
      }
      
    private:
      Dimensions _dimensions;
      PixelShift _pixel_shift;
    };

    class Frame {
    public:
      Frame(FrameHeader header,
            std::vector<uint8_t> color_indices) noexcept :
        _header(std::move(header)),
        _color_indices(std::move(color_indices)) {
      }
      
      Dimensions dimensions() const noexcept {
        return this->_header.dimensions();
      }

      PixelShift pixel_shift() const noexcept {
        return this->_header.pixel_shift();
      }

      void apply_pixel_shift(PixelShift pixel_shift) noexcept {
        this->_header.apply_pixel_shift(pixel_shift);
      }

      const std::vector<uint8_t>& color_indices() const noexcept {
        return this->_color_indices;
      }
      
    private:
      FrameHeader _header;
      std::vector<uint8_t> _color_indices;
    };

    class Animation {
    public:
      Animation(std::vector<Frame> frames,
                uint16_t fps,
                uint16_t action_frame) :
        _frames(std::move(frames)),
        _fps(fps),
        _action_frame(action_frame) {
      }
      
      const std::vector<Frame>& frames() const noexcept {
        return this->_frames;
      }

      Dimensions dimensions() const noexcept {
        Dimensions ret{0, 0};
        for (const Frame& frame : this->frames()) {
          ret = geometry::union_of(ret, frame.dimensions());
        }
        return ret;
      }
      
      uint16_t fps() const noexcept {
        return this->_fps;
      }

      uint16_t action_frame() const noexcept {
        return this->_action_frame;
      }

      size_t num_frames() const noexcept {
        return this->_frames.size();
      }

    private:
      std::vector<Frame> _frames;
      uint16_t _fps;
      uint16_t _action_frame;
    };

    class Orientable {
    public:
      Orientable(std::array<Frame, num_orientations> orientations) :
        _orientations(std::move(orientations)) {
      }

      Dimensions max_dimensions() const noexcept {
        Dimensions ret{0, 0};
        for (Orientation o : orientations) {
          ret = geometry::union_of(ret, this->frame_at(o).dimensions());
        }
        return ret;
      }

      const Frame& frame_at(Orientation o) const noexcept {
	return this->_orientations[o];
      }
    private:
      std::array<Frame, num_orientations> _orientations;
    };

    class OrientableAnimation {
    public:
      OrientableAnimation(std::array<Animation, num_orientations> orientations,
                          uint16_t fps,
                          uint16_t action_frame) :
        _orientations(std::move(orientations)),
        _fps(fps),
        _action_frame(action_frame) {
      }

      Dimensions max_dimensions() const noexcept {
        Dimensions ret{0, 0};
        for (Orientation o : orientations) {
          ret = geometry::union_of(ret, this->animation_at(o).dimensions());
        }
        return ret;
      }
      
      const Animation& animation_at(Orientation o) const noexcept {
        return this->_orientations[o];
      }
      
      uint16_t fps() const noexcept {
        return this->_fps;
      }

      uint16_t action_frame() const noexcept {
        return this->_action_frame;
      }

      size_t num_frames_per_orientation() const noexcept {
        return this->_orientations[0].num_frames();
      }
      
    private:
      std::array<Animation, num_orientations> _orientations;
      uint16_t _fps;
      uint16_t _action_frame;
    };
    
    enum class AnyType {
      frame,
      animation,
      orientable,
      orientable_animation,
    };
    
    class Any {
    public:
      Any(Frame frame) noexcept;
      Any(Animation animation) noexcept;
      Any(Orientable orientable) noexcept;
      Any(OrientableAnimation orientable_animation) noexcept;
      Any(Any&& tmp) noexcept;
      ~Any() noexcept;
      
      AnyType type() const noexcept;
      const Frame& frame_unpack() const;
      const Animation& animation_unpack() const;
      const Orientable& orientable_unpack() const;
      const OrientableAnimation& orientable_animation_unpack() const;
    private:
      AnyType _type;
      union {
	Frame _frame;
	Animation _animation;
	Orientable _orientable;
	OrientableAnimation _orientable_animation;
      };
    };

    Header read_header(std::istream& in);
    Any read_any(std::istream& in);
  }
}
