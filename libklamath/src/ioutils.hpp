#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>

namespace klmth {
  void read(std::istream& in, uint8_t* buf, size_t n);
  
  uint8_t read_u8(std::istream& in);
  
  uint32_t read_le_u32(const uint8_t* buf);
  uint32_t read_le_u32(const uint8_t* buf, size_t& offset);
  uint32_t read_le_u32(std::istream& in);

  uint32_t read_be_u32(const uint8_t* buf);
  uint32_t read_be_u32(const uint8_t* buf, size_t& offset);
  uint32_t read_be_u32(std::istream& in);

  uint16_t read_be_u16(const uint8_t* buf);
  uint16_t read_be_u16(const uint8_t* buf, size_t& offset);
  uint16_t read_be_u16(std::istream& in);

  int16_t read_be_i16(const uint8_t* buf, size_t& offset);
}
