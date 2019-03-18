#include "klamath/frm.hpp"

#include <istream>

#include "src/ioutils.hpp"


namespace {
  using namespace klmth;
  
  const size_t file_header_size =
    4 + 2 + 2 + 2 + (2 * frm::num_orientations) + (2 * frm::num_orientations) + (4 * frm::num_orientations) + 4;
  const size_t frame_header_size = 2 + 2 + 4 + 2 + 2;
  

  void parse_header(const uint8_t* buf, size_t n, klmth::frm::Header& out) {
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
    return std::equal(header.offsets_in_frame_data.begin(),
		      header.offsets_in_frame_data.end(),
		      header.offsets_in_frame_data.begin());
  }

  frm::Image read_frame_as_img(std::istream& in, const frm::PixelShift& shift) {
    frm::Frame frame = frm::read_frame(in);

    frm::PixelShift frame_pixel_shift{ frame.pixel_shift_x, frame.pixel_shift_y };
    frm::PixelShift img_pixel_shift = shift + frame_pixel_shift;

    frm::Rect rect = { frame.width, frame.height };

    return { rect, img_pixel_shift, std::move(frame.color_indices) };
  }
}


klmth::frm::Header klmth::frm::read_header(std::istream& in) {
  std::array<uint8_t, file_header_size> buf;
    
  in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (in.gcount() != buf.size()) {
    throw std::runtime_error("ran out of data while reading frm header");
  }
  
  frm::Header ret;
  parse_header(buf.data(), buf.size(), ret);
  return ret;
}

klmth::frm::Frame klmth::frm::read_frame(std::istream& in) {
  std::array<uint8_t, frame_header_size> buf;

  in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (in.gcount() != buf.size()) {
    throw std::runtime_error("ran out of data while reading frm frame");
  }

  frm::Frame out;
  parse_frame_header(buf.data(), buf.size(), out);

  out.color_indices.resize(out.width * out.height);
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



klmth::frm::Image::Image(Rect rect,
			 PixelShift pixel_shift,
			 std::vector<uint8_t> color_indices) :
  _rect(rect),
  _pixel_shift(pixel_shift),
  _color_indices(std::move(color_indices)) {
}

klmth::frm::Rect klmth::frm::Image::rect() const noexcept {
  return _rect;
}

uint16_t klmth::frm::Image::width() const noexcept {
  return _rect.width;
}

uint16_t klmth::frm::Image::height() const noexcept {
  return _rect.height;
}

klmth::frm::PixelShift klmth::frm::Image::pixel_shift() const noexcept {
  return _pixel_shift;
}

const std::vector<uint8_t>& klmth::frm::Image::color_indices() const noexcept {
  return _color_indices;
}

klmth::frm::Image klmth::frm::read_image(std::istream& in) {  
  Header header = read_header(in);

  if (header.fps > 1) {
    throw std::runtime_error("frm header invalid for image: its 'fps' field is >1, which means the header is for an animation, rather than an image");
  } else if (header.frames_per_direction > 1) {
    throw std::runtime_error("frm header invalid for image: its 'number_of_frames_per_direction' field is >1, which means the header is for an animation, rather than an image");
  } else if (!all_data_offsets_equal(header)) {
    throw std::runtime_error("frm header invalid for image: its 'offsets_into_data' are not all equal, which means the header is for something with multiple orientations (images only have one orientation)");
  }

  PixelShift header_pixel_shift{ header.pixel_shifts_x[0], header.pixel_shifts_y[0] };

  return read_frame_as_img(in, header_pixel_shift);
}



klmth::frm::Animation::Animation(klmth::frm::Rect rect,
				 uint16_t fps,
				 std::vector<Image> frames) :
  _rect(rect),
  _fps(fps),
  _frames(frames) {
}

klmth::frm::Animation klmth::frm::read_animation(std::istream& in) {
  Header header = read_header(in);

  if (header.fps == 1) {
    throw std::runtime_error("frm header invalid for an animation: its 'fps' field is == 1, which means the header is for a static image, rather than an animation");
  } else if (header.frames_per_direction == 1) {
    throw std::runtime_error("frm header invalid for an animation: its 'number_of_frames_per_direction' field is == 1, which means the header is for a static image, rather than an animation");
  } else if (!all_data_offsets_equal(header)) {
    throw std::runtime_error("frm header invalid for an animation: its 'offsets_into_data' are not all equal, which means thie header is for someting with multiple orientations (standard animations only have one orientation)");
  }

  
  PixelShift pixel_shift{ header.pixel_shifts_x[0], header.pixel_shifts_y[0] };
  Rect anim_rect;  // assumed to be the smallest rect that can contain all frames' rects
  std::vector<Image> frames;
  
  for (uint16_t frame = 0; frame < header.frames_per_direction; ++frame) {
    Image img = read_frame_as_img(in, pixel_shift);
    pixel_shift = img.pixel_shift();  // accumulate pixel shifts
    anim_rect.merge(img.rect());
    frames.push_back(std::move(img));
  }

  return { anim_rect, header.fps, std::move(frames) };
}

klmth::frm::Any::Any(Image image) :
  _type(klmth::frm::AnyType::image),
  _image(std::move(image)) {
}

klmth::frm::Any::Any(Any&& tmp) noexcept {
  this->_type = tmp._type;
  switch (tmp._type) {
  case AnyType::image:
    _image = std::move(tmp._image);
    break;
  case AnyType::animation:
    _animation = std::move(tmp._animation);
    break;
  }
}

klmth::frm::Any::~Any() noexcept {
  switch (_type) {
  case AnyType::image:
    _image.~Image();
    break;
  case AnyType::animation:
    _animation.~Animation();
    break;
  }
}

klmth::frm::AnyType klmth::frm::Any::type() const noexcept {
  return _type;
}

const klmth::frm::Image& klmth::frm::Any::image_unpack() const {
  if (_type != AnyType::image) {
    throw std::runtime_error("attempted to unpack a non-image FRM 'any' as an image");
  }

  return _image;
}

const klmth::frm::Animation& klmth::frm::Any::animation_unpack() const {
  if (_type != AnyType::animation) {
    throw std::runtime_error("attempted to unpack a non-animation FRM 'any' as an animation");
  }

  return _animation;
}

klmth::frm::Any klmth::frm::read_any(std::istream& in) {
  Header header = read_header(in);

  if (header.fps == 1) {
    if (all_data_offsets_equal(header)) {
      return read_frame_as_img(in, { header.pixel_shifts_x[0], header.pixel_shifts_y[0] });
    } else {
      throw std::runtime_error("orientable images nyi");
    }
  } else {
    if (all_data_offsets_equal(header)) {
      throw std::runtime_error("animations nyi");
    } else {
      throw std::runtime_error("orientable animations nyi");
    }
  }
}
