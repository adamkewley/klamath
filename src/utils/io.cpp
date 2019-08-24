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

  uint32_t read_le_u32(const uint8_t* buf) {
    uint32_t ret = buf[0];
    ret |= static_cast<uint32_t>(buf[1]) << 8;
    ret |= static_cast<uint32_t>(buf[2]) << 16;
    ret |= static_cast<uint32_t>(buf[3]) << 24;
    return ret;
  }

  uint32_t read_be_u32(const uint8_t* buf) {
    uint32_t ret = static_cast<uint32_t>(buf[0]) << 24;
    ret |= static_cast<uint32_t>(buf[1]) << 16;
    ret |= static_cast<uint32_t>(buf[2]) << 8;
    ret |= buf[3];
    return ret;
  }

  uint16_t read_be_u16(const uint8_t* buf) {
    uint16_t ret = static_cast<uint16_t>(buf[0]) << 8;
    ret |= buf[1];
    return ret;
  }

  int16_t read_be_i16(const uint8_t* buf) {
    return static_cast<int16_t>(::read_be_u16(buf));
  }
}

uint32_t klmth::read_le_u32(std::istream& in) {
  uint8_t buf[4];
  ::read(in, buf, sizeof(buf));
  return ::read_le_u32(buf);
}

uint32_t klmth::read_be_u32(std::istream& in) {
  uint8_t buf[4];
  ::read(in, buf, sizeof(buf));
  return ::read_be_u32(buf);
}

int32_t klmth::read_be_i32(std::istream& in) {
  return read_be_u32(in);
}

uint16_t klmth::read_be_u16(std::istream& in) {
  uint8_t buf[2];
  ::read(in, buf, sizeof(buf));
  return ::read_be_u16(buf);
}

std::vector<int32_t> klmth::read_n_be_i32(std::istream& in, size_t n) {
  std::vector<int32_t> ret(n);
  for (auto i = 0U; i < n; ++i) {
    int32_t global_var = read_be_i32(in);
    ret.push_back(global_var);
  }
  return ret;
}

uint8_t klmth::read_u8(std::istream& in) {
  uint8_t buf[1];
  ::read(in, buf, sizeof(buf));
  return buf[0];
}

std::string klmth::read_str(std::istream& in, size_t len) {
  std::vector<char> buf(len);

  in.read(buf.data(), buf.size());

  if (!in.good()) {
    throw std::runtime_error{"input stream in bad state after reading string"};
  } else if (static_cast<unsigned>(in.gcount()) != buf.size()) {
    throw std::runtime_error{"ran out of data when trying to read a string"};
  }

  std::string ret;
  ret.assign(buf.data(), buf.size());

  return ret;
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

uint8_t klmth::read_u8_unsafe(Cursor& c) {
  return *c.read_then_advance_unsafe(1);
}

uint32_t klmth::read_le_u32(Cursor& c) {
  return ::read_le_u32(c.read_then_advance_unsafe(4));
}

uint32_t klmth::read_be_u32(Cursor& c) {
  return ::read_be_u32(c.read_then_advance_unsafe(4));
}

uint16_t klmth::read_be_u16(Cursor& c) {
  return ::read_be_u16(c.read_then_advance_unsafe(2));
}

int16_t klmth::read_be_i16(Cursor& c) {
  return ::read_be_i16(c.read_then_advance_unsafe(2));
}

std::vector<uint8_t> klmth::read(Cursor& c, size_t n) {
  if (c.remaining() < n) {
    throw std::runtime_error{"prematurely ran out of data"};
  }

  std::vector<uint8_t> ret(n);
  const uint8_t* start = c.read_then_advance_unsafe(n);
  std::memcpy(ret.data(), start, n);

  return ret;
}
