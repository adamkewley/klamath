#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>
#include <vector>

namespace klmth {
  // Cursor points to a userspace buffer, which is known at compile
  // time. Knowing that, and the use of unsafe operations (because
  // upstream code "knows" the size of the buffer) enables high-perf
  // binary parsing with a stream-like interface.
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
  
  uint32_t read_le_u32_unsafe(const uint8_t* buf) noexcept;
  
  uint32_t read_be_u32_unsafe(const uint8_t* buf) noexcept;
  uint16_t read_be_u16_unsafe(const uint8_t* buf) noexcept;
  int16_t read_be_i16_unsafe(const uint8_t* buf) noexcept;

  uint32_t read_le_u32_unsafe(std::istream& in);
  uint32_t read_be_u32_unsafe(std::istream& in);
  int32_t read_be_i32_unsafe(std::istream& in);
  uint16_t read_be_u16_unsafe(std::istream& in);
  std::vector<int32_t> read_n_be_i32(std::istream& in, size_t n);

  uint8_t read_u8_unsafe(Cursor& c) noexcept;
  uint32_t read_le_u32_unsafe(Cursor& c) noexcept;
  uint32_t read_be_u32_unsafe(Cursor& c) noexcept;
  uint16_t read_be_u16_unsafe(Cursor& c) noexcept;
  int16_t read_be_i16_unsafe(Cursor& c) noexcept;
}
