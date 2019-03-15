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

/*
klmth::frm::File klmth::frm::read_file(std::istream& in) {  
  read_header(in);
  read_n_frames(in, 1);
  throw std::runtime_error("NYI");
}
*/
