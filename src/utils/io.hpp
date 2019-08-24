#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>
#include <vector>
#include <cstring>
#include <array>

namespace klmth {
  // Allows for reading from a known buffer, rather than an istream,
  // which reduces virtual function call overhead because you can read
  // all the data up front
  class Cursor {
  public:
    Cursor(const uint8_t* _buf, size_t _size);

    size_t remaining() const noexcept;

    friend uint8_t read_u8_unsafe(Cursor& c);
    friend uint32_t read_le_u32(Cursor& c);
    friend uint32_t read_be_u32(Cursor& c);
    friend uint16_t read_be_u16(Cursor& c);
    friend int16_t read_be_i16(Cursor& c);
    template<size_t N>
    friend std::array<uint8_t, N> read(Cursor &c);
    friend std::vector<uint8_t> read(Cursor& c, size_t n);

  private:
    const uint8_t* data() const noexcept;
    void advance_unsafe(size_t n) noexcept;
    const uint8_t* read_then_advance_unsafe(size_t n) noexcept;

    const uint8_t* buf;
    size_t size;
    size_t offset;
  };

  uint32_t read_le_u32(std::istream& in);
  uint32_t read_be_u32(std::istream& in);
  int32_t read_be_i32(std::istream& in);
  uint16_t read_be_u16(std::istream& in);
  std::vector<int32_t> read_n_be_i32(std::istream& in, size_t n);
  uint8_t read_u8(std::istream& in);
  std::string read_str(std::istream& in, size_t len);

  template<size_t N>
  std::array<uint8_t, N> read(Cursor &c) {
    if (c.remaining() < N) {
      throw std::runtime_error{"prematurely ran out of data"};
    }

    std::array<uint8_t, N> ret;
    const uint8_t* start = c.read_then_advance_unsafe(N);
    std::memcpy(ret.data(), start, N);

    return ret;
  }

  std::vector<uint8_t> read(Cursor& c, size_t n);
  uint8_t read_u8_unsafe(Cursor& c);
  uint32_t read_le_u32(Cursor& c);
  uint32_t read_be_u32(Cursor& c);
  uint16_t read_be_u16(Cursor& c);
  int16_t read_be_i16(Cursor& c);
}
