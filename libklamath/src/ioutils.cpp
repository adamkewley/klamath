#include "ioutils.hpp"

#include <istream>


void klmth::read(std::istream& in, uint8_t* buf, size_t n) {
  in.read(reinterpret_cast<char*>(buf), n);

  if (static_cast<size_t>(in.gcount()) != n) {
    throw std::runtime_error("ran out of data when trying to read a stream");
  }
}

uint8_t klmth::read_u8(std::istream& in) {
  int ret = in.get();

  if (in.eof()) {
    throw std::runtime_error("ran out of data when reading a u8");
  }

  return static_cast<uint8_t>(ret);
}

uint32_t klmth::read_le_u32(const uint8_t* buf) {
  uint32_t ret = buf[0];
  ret |= static_cast<uint32_t>(buf[1]) << 8;
  ret |= static_cast<uint32_t>(buf[2]) << 16;
  ret |= static_cast<uint32_t>(buf[3]) << 24;
  return ret;
}

uint32_t klmth::read_le_u32(const uint8_t* buf, size_t& offset) {
  uint32_t ret = read_le_u32(buf + offset);
  offset += 4;
  return ret;
}

uint32_t klmth::read_le_u32(std::istream& in) {
  uint8_t buf[4];
  read(in, buf, sizeof(buf));
  return read_le_u32(buf);
}

uint32_t klmth::read_be_u32(const uint8_t* buf) {
  uint32_t ret = static_cast<uint32_t>(buf[0]) << 24;
  ret |= static_cast<uint32_t>(buf[1]) << 16;
  ret |= static_cast<uint32_t>(buf[2]) << 8;
  ret |= buf[3];
  return ret;
}

uint32_t klmth::read_be_u32(const uint8_t* buf, size_t& offset) {
  uint32_t ret = read_be_u32(buf + offset);
  offset += 4;
  return ret;
}

uint32_t klmth::read_be_u32(std::istream& in) {
  uint8_t buf[4];
  read(in, buf, sizeof(buf));
  return read_be_u32(buf);
}

uint16_t klmth::read_be_u16(const uint8_t* buf) {
  uint16_t ret = static_cast<uint16_t>(buf[0]) << 8;
  ret |= buf[1];
  return ret;
}

uint16_t klmth::read_be_u16(const uint8_t* buf, size_t& offset) {
  uint16_t ret = read_be_u16(buf + offset);
  offset += 2;
  return ret;
}

uint16_t klmth::read_be_u16(std::istream& in) {
  uint8_t buf[2];
  read(in, buf, sizeof(buf));
  return read_be_u16(buf);
}

int16_t klmth::read_be_i16(const uint8_t* buf, size_t& offset) {
  int16_t ret = static_cast<int16_t>(buf[offset++]) << 8;
  ret |= buf[offset++];
  return ret;
}
