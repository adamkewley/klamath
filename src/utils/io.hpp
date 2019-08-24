#pragma once

#include <cstdint>
#include <cstddef>
#include <istream>
#include <vector>
#include <array>

namespace klmth {
  uint32_t read_le_u32(std::istream& in);
  uint32_t read_be_u32(std::istream& in);
  int32_t read_be_i32(std::istream& in);
  uint16_t read_be_u16(std::istream& in);
  int16_t read_be_i16(std::istream& in);
  std::vector<int32_t> read_n_be_i32(std::istream& in, size_t n);
  uint8_t read_u8(std::istream& in);
  std::string read_str(std::istream& in, size_t len);
  void read(std::istream& in, uint8_t* buf, size_t n);

  template<size_t N>
  std::array<uint8_t, N> read(std::istream& in) {
    std::array<uint8_t, N> ret;
    read(in, ret.data(), ret.size());
    return ret;
  }

  std::vector<uint8_t> read(std::istream& in, size_t n);

  template<size_t N>
  void skip(std::istream& in) {
    std::array<uint8_t, N> buf;
    read(in, buf.data(), buf.size());
  }
}
