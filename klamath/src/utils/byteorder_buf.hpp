#pragma once

#include <cstdint>

namespace klmth {
  uint32_t read_le_u32_unsafe(const uint8_t* buf) noexcept;
  
  uint32_t read_be_u32_unsafe(const uint8_t* buf) noexcept;
  uint16_t read_be_u16_unsafe(const uint8_t* buf) noexcept;
  int16_t read_be_i16_unsafe(const uint8_t* buf) noexcept;
}
