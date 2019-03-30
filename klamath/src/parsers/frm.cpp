#include "src/parsers/frm.hpp"

#include <istream>

#include "src/utils/byteorder.hpp"


using namespace klmth;

namespace {  
  frm::FrameHeader read_frame_header(std::istream& in) {
    static const size_t frame_header_size = 2 + 2 + 4 + 2 + 2;
    
    std::array<uint8_t, frame_header_size> buf;

    in.read(reinterpret_cast<char*>(buf.data()), buf.size());
    if (in.gcount() != buf.size()) {
      throw std::runtime_error("ran out of data while reading frm frame header");
    }

    size_t offset = 0;
    frm::Dimensions dimensions{
        klmth::read_be_u16(buf.data(), offset), 
        klmth::read_be_u16(buf.data(), offset),
    };

    if (klmth::read_be_u32(buf.data(), offset) != geometry::area(dimensions)) {
      throw std::runtime_error("frame header size field does not match the dimensions of the frame");
    }

    frm::PixelShift pixel_shift{
        klmth::read_be_i16(buf.data(), offset),
        klmth::read_be_i16(buf.data(), offset),
    };

    return { dimensions, pixel_shift };
  }

  frm::Frame read_frame_raw(std::istream& in) {
    frm::FrameHeader header = read_frame_header(in);

    std::vector<uint8_t> color_indices(geometry::area(header.dimensions()));

    in.read(reinterpret_cast<char*>(color_indices.data()), color_indices.size());
    if (in.gcount() < 0) {
      throw std::runtime_error("read error when reading frm color indices");
    } else if (static_cast<unsigned>(in.gcount()) != color_indices.size()) {
      throw std::runtime_error("ran out of data when reading color indices: needed = " +
                               std::to_string(color_indices.size()) + " bytes, got = " +
                               std::to_string(in.gcount()) + " bytes");
    }

    return { header, std::move(color_indices) };
  }

  void seek_to_frame_data(std::istream& in, const frm::Header& header, frm::Orientation orient) {
    static const size_t frame_data_start = 0x003e;
    
    in.seekg(frame_data_start + header.offsets_in_frame_data[orient]);
    if (!in.good()) {
      throw std::runtime_error("stream in bad state after seeking to an orientation");
    }
  }

  frm::Frame read_frame(std::istream& in, const frm::PixelShift& shift) {
    frm::Frame frame = read_frame_raw(in);
    frame.apply_pixel_shift(shift);
    return frame;
  }

  frm::Frame seek_and_read_frame(std::istream& in,
                                 const frm::Header& header,
                                 frm::Orientation orient) {
    
    seek_to_frame_data(in, header, orient);
    return read_frame(in, header.pixel_shifts[orient]);
  }

  frm::Animation read_animation(std::istream& in,
                                const frm::Header& header,
                                frm::PixelShift shift) {
    const auto num_frames = header.frames_per_direction;

    std::vector<frm::Frame> frames;
    frames.reserve(num_frames);
    
    for (auto i = 0; i < num_frames; ++i) {
      frm::Frame frame = read_frame(in, shift);
      shift = frame.pixel_shift();
      frames.emplace_back(std::move(frame));
    }

    return { std::move(frames), header.fps, header.action_frame };
  }

  frm::Animation seek_and_read_animation(std::istream& in,
                                         const frm::Header& header,
                                         frm::Orientation orient) {
    seek_to_frame_data(in, header, orient);
    return read_animation(in, header, header.pixel_shifts[orient]);
  }

  frm::Orientable seek_and_read_orientable(std::istream& in, const frm::Header& header) {
    std::array<frm::Frame, frm::num_orientations> _orientations{
      seek_and_read_frame(in, header, frm::north_east),
        seek_and_read_frame(in, header, frm::east),
        seek_and_read_frame(in, header, frm::south_east),
        seek_and_read_frame(in, header, frm::south_west),
        seek_and_read_frame(in, header, frm::west),
        seek_and_read_frame(in, header, frm::north_west),
    };

    return { std::move(_orientations) };    
  }

  frm::OrientableAnimation seek_and_read_orientable_anim(std::istream& in, const frm::Header& header) {
    std::array<frm::Animation, frm::num_orientations> _orientations{
      seek_and_read_animation(in, header, frm::north_east),
        seek_and_read_animation(in, header, frm::east),
        seek_and_read_animation(in, header, frm::south_east),
        seek_and_read_animation(in, header, frm::south_west),
        seek_and_read_animation(in, header, frm::west),
        seek_and_read_animation(in, header, frm::north_west),
    };

    return { std::move(_orientations), header.fps, header.action_frame };
  }

  bool all_data_offsets_equal(const frm::Header& header) {
    const auto& offsets = header.offsets_in_frame_data;
    return std::equal(offsets.begin() + 1, offsets.end(), offsets.begin());
  }
}


frm::Any::Any(Frame frame) noexcept :
_type(AnyType::frame),
  _frame(std::move(frame)) {
}

frm::Any::Any(Animation animation) noexcept :
_type(AnyType::animation),
  _animation(std::move(animation)) {
}

frm::Any::Any(Orientable orientable) noexcept :
_type(AnyType::orientable),
  _orientable(std::move(orientable)) {
}

frm::Any::Any(OrientableAnimation orientable_animation) noexcept :
_type(AnyType::orientable_animation),
  _orientable_animation(std::move(orientable_animation)) {
}

frm::Any::Any(Any&& tmp) noexcept {
  this->_type = tmp._type;

  switch (tmp._type) {
  case AnyType::frame:
    this->_frame = std::move(tmp._frame);
    break;
  case AnyType::animation:
    this->_animation = std::move(tmp._animation);
    break;
  case AnyType::orientable:
    this->_orientable = std::move(tmp._orientable);
    break;
  case AnyType::orientable_animation:
    this->_orientable_animation = std::move(tmp._orientable_animation);
    break;
  }
}

frm::Any::~Any() noexcept {
  switch (_type) {
  case AnyType::frame:
    this->_frame.~Frame();
    break;
  case AnyType::animation:
    this->_animation.~Animation();
    break;
  case AnyType::orientable:
    this->_orientable.~Orientable();
    break;
  case AnyType::orientable_animation:
    this->_orientable_animation.~OrientableAnimation();
    break;
  }
}

frm::AnyType frm::Any::type() const noexcept {
  return this->_type;
}

const frm::Frame& frm::Any::frame_unpack() const {
  if (this->_type != AnyType::frame) {
    throw std::runtime_error("attempted to unpack a non-frame FRM 'any' as a single frame");
  }

  return this->_frame;
}

const frm::Animation& frm::Any::animation_unpack() const {
  if (this->_type != AnyType::animation) {
    throw std::runtime_error("attempted to unpack a non-animation FRM 'any' as an animation");
  }

  return this->_animation;
}

const frm::Orientable& frm::Any::orientable_unpack() const {
  if (_type != AnyType::orientable) {
    throw std::runtime_error("attempted to unpack non-orientable FRM 'any' as an orientable");
  }

  return this->_orientable;
}

const frm::OrientableAnimation& frm::Any::orientable_animation_unpack() const {
  if (this->_type != AnyType::orientable_animation) {
    throw std::runtime_error("attempted to unpack non-animated-orientable FRM 'any' as an animated orientable");
  }

  return _orientable_animation;
}

frm::Header frm::read_header(std::istream& in) {
  static const size_t file_header_size =
    4 + 2 + 2 + 2 + (2 * frm::num_orientations) + (2 * frm::num_orientations) + (4 * frm::num_orientations) + 4;
  
  std::array<uint8_t, file_header_size> buf;

  in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (in.gcount() != buf.size()) {
    throw std::runtime_error("ran out of data while reading an frm header");
  }

  frm::Header out;

  size_t offset = 0;
  out.version_number = klmth::read_be_u32(buf.data(), offset);
  out.fps = klmth::read_be_u16(buf.data(), offset);
  out.action_frame = klmth::read_be_u16(buf.data(), offset);
  out.frames_per_direction = klmth::read_be_u16(buf.data(), offset);
    
  for (auto& pixel_shift : out.pixel_shifts) {
    pixel_shift.x = klmth::read_be_i16(buf.data(), offset);
  }

  for (auto& pixel_shift : out.pixel_shifts) {
    pixel_shift.y = klmth::read_be_i16(buf.data(), offset);
  }

  for (uint32_t& offset_in_frame_data : out.offsets_in_frame_data) {
    offset_in_frame_data = klmth::read_be_u32(buf.data(), offset);
  }

  out.size_of_frame_data = klmth::read_be_u32(buf.data(), offset);

  return out;
}



frm::Any frm::read_any(std::istream& in) {
  Header header = read_header(in);

  if (header.fps > 1) {
    if (all_data_offsets_equal(header)) {
      return seek_and_read_animation(in, header, north_east);
    } else {
      return seek_and_read_orientable_anim(in, header);
    }
  } else {
    if (all_data_offsets_equal(header)) {
      return seek_and_read_frame(in, header, north_east);
    } else {
      return seek_and_read_orientable(in, header);
    }
  }
}
