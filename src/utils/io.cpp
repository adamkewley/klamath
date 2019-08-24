#include "src/utils/io.hpp"

#include <istream>
#include <stdexcept>

namespace {
  void read(std::istream& in, uint8_t* buf, size_t n) {
    in.read(reinterpret_cast<char*>(buf), n);

    if (static_cast<size_t>(in.gcount()) != n) {
      throw std::runtime_error("ran out of data when trying to read a stream");
    }
  }
}

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

uint32_t klmth::read_le_u32_unsafe(std::istream& in) {
  uint8_t buf[4];
  read(in, buf, sizeof(buf));
  return read_le_u32_unsafe(buf);
}

uint32_t klmth::read_be_u32_unsafe(std::istream& in) {
  uint8_t buf[4];
  read(in, buf, sizeof(buf));
  return read_be_u32_unsafe(buf);
}

int32_t klmth::read_be_i32_unsafe(std::istream& in) {
  return read_be_u32_unsafe(in);
}

uint16_t klmth::read_be_u16_unsafe(std::istream& in) {
  uint8_t buf[2];
  read(in, buf, sizeof(buf));
  return read_be_u16_unsafe(buf);
}

std::vector<int32_t> klmth::read_n_be_i32(std::istream& in, size_t n) {
  std::vector<int32_t> ret(n);
  for (auto i = 0U; i < n; ++i) {
    int32_t global_var = read_be_i32_unsafe(in);
    ret.push_back(global_var);
  }
  return ret;
}

uint8_t klmth::read_byte(std::istream& in) {
  uint8_t buf[1];
  read(in, buf, sizeof(buf));
  return buf[0];
}

klmth::Cursor::Cursor(const uint8_t* _buf, size_t _size, size_t _offset) :
  buf(_buf), size(_size), offset(_offset) {
}

klmth::Cursor::Cursor(const uint8_t* _buf, size_t _size) :
  buf(_buf), size(_size), offset(0) {
}

const uint8_t* klmth::Cursor::data() const noexcept {
  return this->buf + this->offset;
}

size_t klmth::Cursor::remaining() const noexcept {
  return this->size - this->offset;
}

void klmth::Cursor::advance_unsafe(size_t n) noexcept {
  this->offset += n;
}

const uint8_t* klmth::Cursor::read_then_advance_unsafe(size_t n) noexcept {
  const uint8_t* ret = this->data();
  this->offset += n;
  return ret;
}

uint8_t klmth::read_u8_unsafe(Cursor& c) noexcept {
  return *c.read_then_advance_unsafe(1);
}

uint32_t klmth::read_le_u32_unsafe(Cursor& c) noexcept {
  return read_le_u32_unsafe(c.read_then_advance_unsafe(4));
}

uint32_t klmth::read_be_u32_unsafe(Cursor& c) noexcept {
  return read_be_u32_unsafe(c.read_then_advance_unsafe(4));
}

uint16_t klmth::read_be_u16_unsafe(Cursor& c) noexcept {
  return read_be_u16_unsafe(c.read_then_advance_unsafe(2));
}

int16_t klmth::read_be_i16_unsafe(Cursor& c) noexcept {
  return read_be_i16_unsafe(c.read_then_advance_unsafe(2));
}
