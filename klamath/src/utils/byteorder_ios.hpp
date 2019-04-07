#pragma once

#include <iosfwd>
#include <cstdint>

namespace klmth {
  uint32_t read_le_u32_unsafe(std::istream& in);
  uint32_t read_be_u32_unsafe(std::istream& in);
  uint16_t read_be_u16_unsafe(std::istream& in);
}
