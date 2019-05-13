#include "src/formats/pal_reader.hpp"

#include <stdexcept>
#include <istream>


namespace {
  const size_t filesize = 0x00008300;  // up to "additional table #1"

  void parse(const uint8_t* buf, size_t n, pal::File& out) {
    if (n < filesize) {
      throw std::runtime_error("ran out of data when reading pal file");
    }

    size_t offset = 0;
    for (klmth::Rgb& rgb : out.palette) {
      rgb.r = buf[offset++];
      rgb.g = buf[offset++];
      rgb.b = buf[offset++];
    }

    for (uint8_t& conversion : out.rgb_conversion_table) {
      conversion = buf[offset++];
    }
  }
}

pal::File pal::parse(std::istream& in) {
  std::array<uint8_t, filesize> buf;
  in.read(reinterpret_cast<char*>(buf.data()), buf.size());

  if (in.gcount() != buf.size()) {
    throw std::runtime_error("insufficient data in palette (.pal) source: required size is " + std::to_string(filesize) + " bytes");
  }

  pal::File palette;
  ::parse(buf.data(), buf.size(), palette);
  return palette;
}
