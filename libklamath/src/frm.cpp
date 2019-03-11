#include "klamath/frm.hpp"

#include <istream>

#include "src/ioutils.hpp"


namespace {
  using namespace klmth;
  
  const size_t file_header_size =
    4 + 2 + 2 + 2 + (2 * frm::num_orientations) + (2 * frm::num_orientations) + (4 * frm::num_orientations) + 4;
}

void klmth::frm::parse_header(const uint8_t* buf, size_t n, klmth::frm::Header& out) {
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

klmth::frm::Header klmth::frm::parse_header(std::istream& frm_in) {
  std::array<uint8_t, file_header_size> buf;
    
  frm_in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (frm_in.gcount() != buf.size()) {
    throw std::runtime_error("ran out of data while reading frm header");
  }
  
  frm::Header ret;
  frm::parse_header(buf.data(), buf.size(), ret);
  return ret;
}

void klmth::frm::parse_frame(const uint8_t* buf, size_t n, Frame& out) {
  static const size_t header_size = 2 + 2 + 4 + 2 + 2;

  if (n < header_size) {
    throw std::runtime_error("ran out of data when reading frm data");
  }

  size_t offset = 0;
  out.width = klmth::read_be_u16(buf, offset);
  out.height = klmth::read_be_u16(buf, offset);
  out.size = klmth::read_be_u32(buf, offset);
  out.pixel_shift_x = klmth::read_be_i16(buf, offset);
  out.pixel_shift_y = klmth::read_be_i16(buf, offset);

  if (n < (offset + out.size)) {
    throw std::runtime_error("ran out of data when reading frm color indices");
  }

  out.color_indices.resize(out.size);
  out.color_indices.insert(out.color_indices.begin(), buf + offset, buf + offset + out.size);
}
