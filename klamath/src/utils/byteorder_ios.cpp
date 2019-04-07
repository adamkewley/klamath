#include "src/utils/byteorder_ios.hpp"

#include <istream>

#include "src/utils/byteorder_buf.hpp"


namespace {
  void read(std::istream& in, uint8_t* buf, size_t n) {
    in.read(reinterpret_cast<char*>(buf), n);

    if (static_cast<size_t>(in.gcount()) != n) {
      throw std::runtime_error("ran out of data when trying to read a stream");
    }
  }
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

uint16_t klmth::read_be_u16_unsafe(std::istream& in) {
  uint8_t buf[2];
  read(in, buf, sizeof(buf));
  return read_be_u16_unsafe(buf);
}
