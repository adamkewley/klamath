#include "src/utils/byteorder_buf.hpp"


uint32_t klmth::read_le_u32_unsafe(const uint8_t* buf) noexcept {
  uint32_t ret = buf[0];
  ret |= static_cast<uint32_t>(buf[1]) << 8;
  ret |= static_cast<uint32_t>(buf[2]) << 16;
  ret |= static_cast<uint32_t>(buf[3]) << 24;
  return ret;
}

uint32_t klmth::read_be_u32_unsafe(const uint8_t* buf) noexcept {
  uint32_t ret = static_cast<uint32_t>(buf[0]) << 24;
  ret |= static_cast<uint32_t>(buf[1]) << 16;
  ret |= static_cast<uint32_t>(buf[2]) << 8;
  ret |= buf[3];
  return ret;
}

uint16_t klmth::read_be_u16_unsafe(const uint8_t* buf) noexcept {
  uint16_t ret = static_cast<uint16_t>(buf[0]) << 8;
  ret |= buf[1];
  return ret;
}

int16_t klmth::read_be_i16_unsafe(const uint8_t* buf) noexcept {
  return static_cast<int16_t>(read_be_u16_unsafe(buf));
}
