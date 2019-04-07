#include "src/utils/cursor.hpp"

#include <stdexcept>

#include "src/utils/byteorder_buf.hpp"


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

void klmth::Cursor::advance(size_t n) {
  if (n > this->remaining()) {
    throw std::out_of_range("tried to advance a cursor beyond its bounds");
  } else {
    this->advance_unsafe(n);
  }
}

void klmth::Cursor::advance_unsafe(size_t n) noexcept {
  this->offset += n;
}

uint8_t klmth::read_u8_unsafe(Cursor& c) noexcept {
  uint8_t ret = c.data()[0];
  c.advance_unsafe(1);
  return ret;
}

uint32_t klmth::read_le_u32_unsafe(Cursor& c) noexcept {
  uint32_t ret = read_le_u32_unsafe(c.data());
  c.advance_unsafe(4);
  return ret;
}

uint32_t klmth::read_be_u32_unsafe(Cursor& c) noexcept {
  uint32_t ret = read_be_u32_unsafe(c.data());
  c.advance_unsafe(4);
  return ret;
}

uint16_t klmth::read_be_u16_unsafe(Cursor& c) noexcept {
  uint16_t ret = read_be_u16_unsafe(c.data());
  c.advance(2);
  return ret;
}

int16_t klmth::read_be_i16_unsafe(Cursor& c) noexcept {
  int16_t ret = read_be_i16_unsafe(c.data());
  c.advance(2);
  return ret;
}
