#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <iosfwd>

#include "klamath/geometry.hpp"


namespace klmth {
  namespace frm {
    
    enum Orientation : unsigned char {
      north_east = 0,
      east = 1,
      south_east = 2,
      south_west = 3,
      west = 4,
      north_west = 5,
      num_orientations = 6,
    };

    const std::array<Orientation, num_orientations> orientations {
      north_east,
      east,
      south_east,
      south_west,
      west,
      north_west,
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

    struct PixelShift {
      int16_t x;
      int16_t y;

      PixelShift operator+(PixelShift other) const noexcept {
	int16_t x = this->x + other.x;
	int16_t y = this->y + other.y;

	return { x, y };
      }
      
      PixelShift& operator+=(PixelShift other) noexcept {
	this->x += other.x;
	this->y += other.y;
	return *this;
      }
    };

    using Dimensions = geometry::Dimensions<uint16_t>;
    
    struct Image {
      Dimensions dimensions;
      PixelShift pixel_shift;
      std::vector<uint8_t> color_indices;
    };

    struct Animation {
      Dimensions dimensions;
      uint16_t fps;
      std::vector<Image> frames;

      size_t num_frames() const noexcept {
	return this->frames.size();
      }
    };

    struct Orientable {
      Dimensions dimensions;
      std::array<Image, num_orientations> orientations;

      const Image& image_at(Orientation o) const noexcept {
	return this->orientations[o];
      }
    };

    struct AnimatedOrientable {
      Dimensions dimensions;
      uint16_t frames_per_direction;
      uint16_t fps;
      std::array<Animation, num_orientations> orientations;

      const Animation& animation_at(Orientation o) const noexcept {
	return this->orientations[o];
      }
    };
    
    enum class AnyType {
      image,
      animation,
      orientable,
      animated_orientable,
    };
    
    class Any {
    public:
      Any(Image image) noexcept;
      Any(Animation animation) noexcept;
      Any(Orientable orientable) noexcept;
      Any(AnimatedOrientable animated_orientable) noexcept;
      Any(Any&& tmp) noexcept;
      ~Any() noexcept;
      
      AnyType type() const noexcept;
      const Image& image_unpack() const;
      const Animation& animation_unpack() const;
      const Orientable& orientable_unpack() const;
      const AnimatedOrientable& animated_orientable_unpack() const;
    private:
      AnyType _type;
      union {
	Image _image;
	Animation _animation;
	Orientable _orientable;
	AnimatedOrientable _animated_orientable;
      };
    };

    Frame read_frame(std::istream& in);
    Header read_header(std::istream& in);
    Any read_any(std::istream& in);
  }
}
