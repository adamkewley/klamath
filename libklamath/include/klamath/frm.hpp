#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <iosfwd>


namespace klmth {
  namespace frm {
    
    enum class Orientation {
      north_east,
      east,
      south_east,
      south_west,
      west,
      north_west,
    };

    static const unsigned num_orientations = 6;

    const std::array<Orientation, num_orientations> orientations {
      Orientation::north_east,
      Orientation::east,
      Orientation::south_east,
      Orientation::south_west,
      Orientation::west,
      Orientation::north_west,
    };

    constexpr unsigned orientation_index(Orientation orientation) {
      switch (orientation) {
      case Orientation::north_east:
	return 0;
      case Orientation::east:
	return 1;
      case Orientation::south_east:
	return 2;
      case Orientation::south_west:
	return 3;
      case Orientation::west:
	return 4;
      case Orientation::north_west:
	return 5;
      default:
	throw std::runtime_error("unknown orientation");
      }
    }
    
    
    // low-level API: used when clients want to read particular parts
    // of an FRM file and deal with the resulting data directly.
    
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


    // high-level API: used when clients just want to load FRM files
    // into abstractions such as images, animations, etc.

    struct PixelShift {
      int16_t x;
      int16_t y;

      PixelShift() noexcept;
      PixelShift(int16_t _x, int16_t _y) noexcept;

      PixelShift operator+(const PixelShift& other) const noexcept;
    };

    
    struct Dimensions {
      uint16_t width;
      uint16_t height;

      Dimensions() noexcept;
      Dimensions(uint16_t _width, uint16_t _height) noexcept;

      Dimensions union_with(const Dimensions& other) const noexcept;
    };

    
    class Image {
    public:
      Image() noexcept;
      Image(Dimensions dimensions,
	    PixelShift pixel_shift,
	    std::vector<uint8_t> color_indices) noexcept;

      Dimensions dimensions() const noexcept;
      uint16_t width() const noexcept;
      uint16_t height() const noexcept;
      PixelShift pixel_shift() const noexcept;
      const std::vector<uint8_t>& color_indices() const noexcept;
    private:
      Dimensions _dimensions;
      PixelShift _pixel_shift;
      std::vector<uint8_t> _color_indices;
    };


    class Animation {
    public:
      Animation(Dimensions dimensions,
		uint16_t fps,
		std::vector<Image> frames);

      Dimensions dimensions() const noexcept;
      uint16_t width() const noexcept;
      uint16_t height() const noexcept;
      uint16_t fps() const noexcept;
      size_t num_frames() const noexcept;
      const std::vector<Image>& frames() const noexcept;
    private:
      Dimensions _dimensions;
      uint16_t _fps;
      std::vector<Image> _frames;
    };


    class Orientable {
    public:
      Orientable(Dimensions dimensions,
		 std::array<Image, num_orientations> orientations) noexcept;
      uint16_t width() const noexcept;
      uint16_t height() const noexcept;
      Dimensions dimensions() const noexcept;
      const Image& image_at(const Orientation& o) const noexcept;
      
    private:
      Dimensions _dimensions;
      std::array<Image, num_orientations> _orientations;
    };

    
    class AnimatedOrientable {
    public:
      AnimatedOrientable(Dimensions dimensions,
			 std::array<Animation, num_orientations> frames);

      Dimensions dimensions() const noexcept;
      const Animation& animation_at(const Orientation& o) const noexcept;
    private:
      Dimensions _dimensions;
      std::array<Animation, num_orientations> _orientations;
    };
    

    // For when the caller doesn't know what type the FRM file is and
    // wants to defer the decision to runtime.
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

    Any read_any(std::istream& in);
  }
}
