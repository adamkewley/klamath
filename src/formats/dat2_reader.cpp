#include "src/formats/dat2_reader.hpp"

#include <istream>
#include <vector>
#include <array>

#include "src/utils/io.hpp"

using klmth::read_le_u32;
using namespace dat2;


Sections dat2::read_sections(std::istream& in) {
  constexpr unsigned num_files_field_len = 4;
  constexpr unsigned tree_size_field_len = 4;
  constexpr unsigned file_size_field_len = 4;
  constexpr unsigned footer_field_len = tree_size_field_len + file_size_field_len;

  in.seekg(-static_cast<int>(footer_field_len), in.end);

  if (in.bad()) {
    throw std::runtime_error("could not seek to the end of a dat2 file");
  } else if (in.tellg() <= num_files_field_len) {
    throw std::runtime_error("insufficient data in dat2 file: must be at least 12 bytes (num files section, tree size section, and file size section");
  }

  uint32_t tree_size = read_le_u32(in);
  uint32_t file_size = read_le_u32(in);

  if (file_size != in.tellg()) {
    throw std::runtime_error("filesize declared in DAT2 footer does not match actual size of the file");
  } else if (tree_size > file_size) {
    throw std::runtime_error("tree size too large for DAT2 file");
  }

  uint32_t tree_offset = file_size - file_size_field_len - tree_size;
  uint32_t num_files_offset = tree_offset - num_files_field_len;
  uint32_t data_section_size = num_files_offset;

  in.seekg(num_files_offset);
  uint32_t num_files = read_le_u32(in);

  return { data_section_size, num_files, tree_offset, tree_size, file_size };
}

TreeEntry dat2::read_tree_entry(std::istream& in) {
  constexpr unsigned is_compressed_len = 1;
  constexpr unsigned decompressed_size_len = 4;
  constexpr unsigned packed_size_len = 4;
  constexpr unsigned offset_len = 4;
  constexpr unsigned fixed_fields_len =
    is_compressed_len + decompressed_size_len + packed_size_len + offset_len;

  TreeEntry out;
  
  uint32_t filename_len = read_le_u32(in);

  std::vector<uint8_t> filename_buf;
  filename_buf.resize(filename_len);

  in.read(reinterpret_cast<char*>(filename_buf.data()), filename_buf.size());
  if (!in.good()) {
    throw std::runtime_error("input stream in bad state after reading dat2 entry filename");
  } else if (static_cast<unsigned>(in.gcount()) != filename_buf.size()) {
    throw std::runtime_error("ran out of data when trying to read a dat2 entry filename");
  }

  out.filename.assign(reinterpret_cast<char*>(filename_buf.data()), filename_buf.size());

  std::array<uint8_t, fixed_fields_len> buf;
  in.read(reinterpret_cast<char*>(buf.data()), buf.size());
  if (!in.good()) {
    throw std::runtime_error("input stream in bad state after reading dat2 entry fields");
  } else if (static_cast<unsigned>(in.gcount() != buf.size())) {
    throw std::runtime_error("ran out of data when trying to read dat2 entry fields");
  }

  klmth::Cursor c{buf.data(), buf.size()};
  out.is_compressed = read_u8_unsafe(c) != 0;
  out.decompressed_size = read_le_u32(c);
  out.packed_size = read_le_u32(c);
  out.offset = read_le_u32(c);

  return out;
}
