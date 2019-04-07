#pragma once

#include <cstdint>
#include <cstddef>

namespace klmth {
  
  class Cursor {
  public:
    Cursor(const uint8_t* _buf, size_t _size, size_t _offset);
    Cursor(const uint8_t* _buf, size_t _size);

    const uint8_t* data() const noexcept;
    size_t remaining() const noexcept;
    void advance_unsafe(size_t n) noexcept;
    const uint8_t* read_then_advance_unsafe(size_t n) noexcept;
  private:
    const uint8_t* buf;
    size_t size;
    size_t offset;
  };

  uint8_t read_u8_unsafe(Cursor& c) noexcept;
  uint32_t read_le_u32_unsafe(Cursor& c) noexcept;
  uint32_t read_be_u32_unsafe(Cursor& c) noexcept;
  uint16_t read_be_u16_unsafe(Cursor& c) noexcept;
  int16_t read_be_i16_unsafe(Cursor& c) noexcept;
}
