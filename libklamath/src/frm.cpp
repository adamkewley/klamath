#include "klamath/frm.hpp"

#include <istream>

#include "src/ioutils.hpp"


using namespace klmth;

namespace {  
  const size_t file_header_size =
    4 + 2 + 2 + 2 + (2 * frm::num_orientations) + (2 * frm::num_orientations) + (4 * frm::num_orientations) + 4;
  const size_t frame_header_size = 2 + 2 + 4 + 2 + 2;
  

  void parse_header(const uint8_t* buf, size_t n, frm::Header& out) {
    if (n < file_header_size) {
      throw std::runtime_error("ran out of data when reading an frm header");
    }

    size_t offset = 0;
    out.version_number = klmth::read_be_u32(buf, offset);
    out.fps = klmth::read_be_u16(buf, offset);
    out.action_frame = klmth::read_be_u16(buf, offset);
    out.frames_per_direction = klmth::read_be_u16(buf, offset);

    for (int16_t& pixel_shift_x : out.pixel_shifts_x) {
      pixel_shift_x = klmth::read_be_i16(buf, offset);
    }

    for (int16_t& pixel_shift_y : out.pixel_shifts_y) {
      pixel_shift_y = klmth::read_be_i16(buf, offset);
    }

    for (uint32_t& offset_in_frame_data : out.offsets_in_frame_data) {
      offset_in_frame_data = klmth::read_be_u32(buf, offset);
    }

    out.size_of_frame_data = klmth::read_be_u32(buf, offset);
  }

  void parse_frame_header(const uint8_t* buf, size_t n, frm::Frame& out) {
    if (n < frame_header_size) {
      throw std::runtime_error("ran out of data when reading frm data");
    }

    size_t offset = 0;
    out.width = klmth::read_be_u16(buf, offset);
    out.height = klmth::read_be_u16(buf, offset);
    out.size = klmth::read_be_u32(buf, offset);
    out.pixel_shift_x = klmth::read_be_i16(buf, offset);
    out.pixel_shift_y = klmth::read_be_i16(buf, offset);
  }

  bool all_data_offsets_equal(const frm::Header& header) {
    auto first = header.offsets_in_frame_data[0];
    for (size_t i = 1; i < header.offsets_in_frame_data.size(); ++i) {
      if (header.offsets_in_frame_data[i] != first) {
	return false;
      }
    }
    return true;
  }

  frm::Image read_frame_as_img(std::istream& in, frm::PixelShift shift) {
    frm::Frame frame = frm::read_frame(in);

    frm::PixelShift frame_pixel_shift{ frame.pixel_shift_x, frame.pixel_shift_y };
    frm::PixelShift img_pixel_shift = shift + frame_pixel_shift;

    frm::Dimensions dimensions{ frame.width, frame.height };

    return { dimensions, img_pixel_shift, std::move(frame.color_indices) };
  }

  frm::Animation read_frames_as_animation(std::istream& in, frm::Header header) {  
    frm::PixelShift pixel_shift{ header.pixel_shifts_x[0], header.pixel_shifts_y[0] };
    frm::Dimensions anim_dimensions;
    std::vector<frm::Image> frames;
    frames.reserve(header.frames_per_direction);
  
    for (uint16_t frame = 0; frame < header.frames_per_direction; ++frame) {
      frm::Image img = read_frame_as_img(in, pixel_shift);
      pixel_shift += img.pixel_shift;
      anim_dimensions = union_of(anim_dimensions, img.dimensions);
      frames.push_back(std::move(img));
    }

    return { anim_dimensions, header.fps, std::move(frames) };
  }

  frm::Orientable read_frame_data_as_orientable(std::istream& in, frm::Header header) {
    std::array<frm::Image, frm::num_orientations> _orientations{};
    frm::Dimensions dimensions;
    frm::PixelShift pixel_shift{ header.pixel_shifts_x[0], header.pixel_shifts_y[0] };

    for (frm::Orientation o : frm::orientations) {
      frm::Image img = read_frame_as_img(in, pixel_shift);
	
      pixel_shift += img.pixel_shift;
      dimensions = geometry::union_of(dimensions, img.dimensions);
      _orientations[o] = std::move(img);
    }
      
    return { dimensions, std::move(_orientations) };
  }

  frm::AnimatedOrientable read_frame_data_as_anim_orientable(std::istream& in, frm::Header header) {
    std::array<frm::Animation, frm::num_orientations> _orientations{
      read_frames_as_animation(in, header),
	read_frames_as_animation(in, header),
	read_frames_as_animation(in, header),
	read_frames_as_animation(in, header),
	read_frames_as_animation(in, header),
	read_frames_as_animation(in, header),
    };

    frm::Dimensions d;
    for (const frm::Animation& anim : _orientations) {
      d = geometry::union_of(d, anim.dimensions);
    }

    return { d, header.frames_per_direction, header.fps, std::move(_orientations) };
  }
}


frm::Any::Any(Image image) noexcept :
  _type(AnyType::image),
  _image(std::move(image)) {
}

frm::Any::Any(Animation animation) noexcept :
  _type(AnyType::animation),
  _animation(std::move(animation)) {
}

frm::Any::Any(Orientable orientable) noexcept :
  _type(AnyType::orientable),
  _orientable(std::move(orientable)) {
}

frm::Any::Any(AnimatedOrientable animated_orientable) noexcept :
  _type(AnyType::animated_orientable),
  _animated_orientable(std::move(animated_orientable)) {
  }

frm::Any::Any(Any&& tmp) noexcept {
  this->_type = tmp._type;
  
  switch (tmp._type) {
  case AnyType::image:
    this->_image = std::move(tmp._image);
    break;
  case AnyType::animation:
    this->_animation = std::move(tmp._animation);
    break;
  case AnyType::orientable:
    this->_orientable = std::move(tmp._orientable);
    break;
  case AnyType::animated_orientable:
    this->_animated_orientable = std::move(tmp._animated_orientable);
    break;
  }
}

frm::Any::~Any() noexcept {
  switch (_type) {
  case AnyType::image:
    _image.~Image();
    break;
  case AnyType::animation:
    _animation.~Animation();
    break;
  case AnyType::orientable:
    _orientable.~Orientable();
    break;
  case AnyType::animated_orientable:
    _animated_orientable.~AnimatedOrientable();
    break;
  }
}

frm::AnyType frm::Any::type() const noexcept {
  return _type;
}

const frm::Image& frm::Any::image_unpack() const {
  if (_type != AnyType::image) {
    throw std::runtime_error("attempted to unpack a non-image FRM 'any' as an image");
  }

  return _image;
}

const frm::Animation& frm::Any::animation_unpack() const {
  if (_type != AnyType::animation) {
    throw std::runtime_error("attempted to unpack a non-animation FRM 'any' as an animation");
  }

  return _animation;
}

const frm::Orientable& frm::Any::orientable_unpack() const {
  if (_type != AnyType::orientable) {
    throw std::runtime_error("attempted to unpack non-orientable FRM 'any' as an orientable");
  }

  return _orientable;
}

const frm::AnimatedOrientable& frm::Any::animated_orientable_unpack() const {
  if (_type != AnyType::animated_orientable) {
    throw std::runtime_error("attempted to unpack non-animated-orientable FRM 'any' as an animated orientable");
  }

  return _animated_orientable;
}

frm::Header frm::read_header(std::istream& in) {
  std::array<uint8_t, file_header_size> buf;
    
  in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (in.gcount() != buf.size()) {
    throw std::runtime_error("ran out of data while reading frm header");
  }
  
  frm::Header ret;
  parse_header(buf.data(), buf.size(), ret);
  return ret;
}

frm::Frame frm::read_frame(std::istream& in) {
  std::array<uint8_t, frame_header_size> buf;

  in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (in.gcount() != buf.size()) {
    throw std::runtime_error("ran out of data while reading frm frame");
  }

  frm::Frame out;
  parse_frame_header(buf.data(), buf.size(), out);

  out.color_indices.resize(out.size);
  in.read(reinterpret_cast<char*>(out.color_indices.data()), out.color_indices.size());
  if (in.gcount() < 0) {
    throw std::runtime_error("read error when reading frm color indices");
  } else if (static_cast<unsigned>(in.gcount()) != out.color_indices.size()) {
    throw std::runtime_error("ran out of data when reading color indices: needed = " +
			     std::to_string(out.color_indices.size()) + " bytes, got = " +
			     std::to_string(in.gcount()) + " bytes");
  }

  return out;
}

frm::Any frm::read_any(std::istream& in) {
  Header header = read_header(in);

  if (header.fps > 1) {
    if (all_data_offsets_equal(header)) {
      return read_frames_as_animation(in, std::move(header));
    } else {
      return read_frame_data_as_anim_orientable(in, std::move(header));
    }
  } else {
    if (all_data_offsets_equal(header)) {
      return read_frame_as_img(in, { header.pixel_shifts_x[0], header.pixel_shifts_y[0] });
    } else {
      return read_frame_data_as_orientable(in, std::move(header));
    }
  }
}
