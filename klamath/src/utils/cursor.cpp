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
