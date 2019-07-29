#include "src/formats/frm_reader.hpp"

#include <istream>
#include <algorithm>
#include <string>

#include "src/utils/io.hpp"

using klmth::read_be_u32_unsafe;
using klmth::read_be_u16_unsafe;
using klmth::read_be_i16_unsafe;
using klmth::geometry::area;

namespace {

  frm::Frame read_frame(std::istream& in, frm::PixelShift base_shift) {
    constexpr size_t frame_header_size = 2 + 2 + 4 + 2 + 2;
    
    std::array<uint8_t, frame_header_size> buf;

    in.read(reinterpret_cast<char*>(buf.data()), buf.size());
    if (in.gcount() != buf.size()) {
      throw std::runtime_error("ran out of data while reading frm frame header");
    }
    klmth::Cursor c{buf.data(), buf.size()};
    
    frm::Dimensions dimensions{
        read_be_u16_unsafe(c), 
        read_be_u16_unsafe(c),
    };

    if (read_be_u32_unsafe(c) != area(dimensions)) {
      throw std::runtime_error("frame header size field does not match the dimensions of the frame");
    }

    frm::PixelShift pixel_shift{
        read_be_i16_unsafe(c),
        read_be_i16_unsafe(c),
    };
    pixel_shift += base_shift;

    std::vector<uint8_t> color_indices(area(dimensions));

    in.read(reinterpret_cast<char*>(color_indices.data()), color_indices.size());
    if (in.gcount() < 0) {
      throw std::runtime_error("read error when reading frm color indices");
    } else if (static_cast<unsigned>(in.gcount()) != color_indices.size()) {
      throw std::runtime_error("ran out of data when reading color indices: needed = " +
                               std::to_string(color_indices.size()) + " bytes, got = " +
                               std::to_string(in.gcount()) + " bytes");
    }

    return { dimensions, pixel_shift, std::move(color_indices) };
  }

  frm::Animation read_animation(std::istream& in,
                                frm::Orientation o,
                                const frm::Header& h) {

    std::vector<frm::Frame> frames;    
    frm::PixelShift ps = h.pixel_shifts[o];
    
    for (auto i = 0U; i < h.frames_per_direction; ++i) {
      frm::Frame f = read_frame(in, ps);
      ps = f.pixel_shift();
      frames.push_back(std::move(f));
    }

    return { h.fps, h.action_frame, std::move(frames) };
  }
}

frm::Header frm::read_header(std::istream& in) {
  constexpr size_t file_header_size =
    4 + 2 + 2 + 2 + (2 * frm::num_orientations) + (2 * frm::num_orientations) + (4 * frm::num_orientations) + 4;
  
  std::array<uint8_t, file_header_size> buf;

  in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (in.gcount() != buf.size()) {
    throw std::runtime_error("ran out of data while reading an frm header");
  }

  klmth::Cursor c{buf.data(), buf.size()};
  uint32_t version_number = read_be_u32_unsafe(c);
  uint16_t fps = read_be_u16_unsafe(c);
  uint16_t action_frame = read_be_u16_unsafe(c);
  uint16_t frames_per_direction = read_be_u16_unsafe(c);

  std::array<PixelShift, frm::num_orientations> pixel_shifts;
  for (auto& pixel_shift : pixel_shifts) {
    pixel_shift.x = read_be_i16_unsafe(c);
  }

  for (auto& pixel_shift : pixel_shifts) {
    pixel_shift.y = read_be_i16_unsafe(c);
  }
  
  std::array<uint32_t, frm::num_orientations> offsets_in_frame_data;
  for (uint32_t& offset_in_frame_data : offsets_in_frame_data) {
    offset_in_frame_data = read_be_u32_unsafe(c);
  }

  uint32_t size_of_frame_data = read_be_u32_unsafe(c);

  return {
    version_number,
    fps,
    action_frame,
    frames_per_direction,
    pixel_shifts,
    offsets_in_frame_data,
    size_of_frame_data
  };
}

frm::File frm::read_file(std::istream& in) {
  frm::Header h = read_header(in);

  
  uint32_t max_offset =
    *std::max_element(h.offsets_in_frame_data.begin(), h.offsets_in_frame_data.end());

  if (max_offset > h.size_of_frame_data) {
    throw std::runtime_error("invalid frm data: the header contains an offset that is greater than the size of the frm's frame data");
  }

  auto data_start = in.tellg();
  {
    in.seekg(h.size_of_frame_data, std::ios_base::cur);
    auto data_end = in.tellg();

    if (data_end - data_start < h.size_of_frame_data) {
      throw std::runtime_error("invalid frm data: the size of the frame data (indicated in header) is smaller than the provided data");
    }
  }

  if (h.frames_per_direction > 0 && h.size_of_frame_data == 0) {
    throw std::runtime_error("invalid frm data: nonzero number of frames per orientation but there is no frame data in the file");
  }
  

  const std::array<uint32_t, num_orientations>& offsets =
    h.offsets_in_frame_data;
  
  std::vector<Animation> animations;
  std::array<uint8_t, num_orientations> animation_idxs{};

  for (frm::Orientation orient : frm::orientations) {
    const uint32_t offset = offsets[orient];

    const auto offsets_begin = offsets.begin();
    const auto offsets_cur = offsets_begin + orient;
    
    const auto offsets_prev = std::find(offsets_begin, offsets_cur, offset);

    if (offsets_prev == offsets_cur) {
      // first time offset encountered, load corresponding animation
      // located at offset
      std::streamoff anim_data_offset =
        data_start + static_cast<std::streamoff>(offset);
      in.seekg(anim_data_offset, std::ios_base::beg);

      animations.emplace_back(read_animation(in, orient, h));
      
      animation_idxs[orient] =
        std::distance(offsets_begin, offsets_cur);
    } else {
      // offset previously encountered, reference previously loaded
      // animation
      animation_idxs[orient] =
        std::distance(offsets_begin, offsets_prev);
    }
  }

  return { std::move(animations), std::move(animation_idxs) };
}
