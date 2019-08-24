#include "src/formats/pal_reader.hpp"

#include <stdexcept>

#include "src/utils/io.hpp"


namespace {
  const size_t filesize = 0x00008300;  // up to "additional table #1"

  void parse(const uint8_t* buf, size_t n, pal::File& out) {
    if (n < filesize) {
      throw std::runtime_error("ran out of data when reading pal file");
    }

    size_t offset = 0;
    for (pal::Rgb& rgb : out.palette) {
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
  klmth::read(in, buf.data(), buf.size());

  pal::File palette;
  ::parse(buf.data(), buf.size(), palette);
  return palette;
}
