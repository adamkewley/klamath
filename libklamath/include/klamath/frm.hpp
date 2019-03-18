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


    // high-level API: used when clients want to load FRM files into
    // abstractions such as images, animations, etc.

    struct PixelShift {
      int16_t x;
      int16_t y;

      PixelShift(int16_t _x, int16_t _y) : x(_x), y(_y) {
      }

      PixelShift operator+(const PixelShift& other) const noexcept {
	return {
	  static_cast<int16_t>(this->x + other.x),
	  static_cast<int16_t>(this->y + other.y),
	};
      }
    };

    struct Rect {
      uint16_t width;
      uint16_t height;

      Rect() : width(0), height(0) {
      }
      
      Rect(uint16_t _width, uint16_t _height) : width(_width), height(_height) {
      }

      Rect merge(const Rect& other) {
	return { std::max(this->width, other.width), std::max(this->height, other.height) };
      }
    };
    
    class Image {
    public:
      Image(Rect rect,
	    PixelShift pixel_shift,
	    std::vector<uint8_t> color_indices);

      Rect rect() const noexcept;
      uint16_t width() const noexcept;
      uint16_t height() const noexcept;
      PixelShift pixel_shift() const noexcept;
      const std::vector<uint8_t>& color_indices() const noexcept;
    private:
      Rect _rect;
      PixelShift _pixel_shift;
      std::vector<uint8_t> _color_indices;
    };

    Image read_image(std::istream& in);


    class Animation {
    public:
      Animation(Rect rect,
		uint16_t fps,
		std::vector<Image> frames);
		
      Rect rect() const noexcept;
      uint16_t fps() const noexcept;      
      const std::vector<Image>& frames() const noexcept;
    private:
      Rect _rect;
      uint16_t _fps;
      std::vector<Image> _frames;
    };

    Animation read_animation(std::istream& in);


    // For when the caller doesn't know what type the FRM file is and
    // wants to defer the decision to runtime.
    enum class AnyType {
      image,
      animation,
    };
    
    class Any {
    public:
      Any(Image image);
      Any(Animation animation);
      Any(Any&& tmp) noexcept;
      ~Any() noexcept;
      
      AnyType type() const noexcept;
      const Image& image_unpack() const;
      const Animation& animation_unpack() const;
    private:
      AnyType _type;
      union {
	Image _image;
	Animation _animation;
      };
    };

    Any read_any(std::istream& in);
  }
}
