#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>

namespace klmth {
  namespace zlib {
    void decompress(const uint8_t* in, size_t in_size, uint8_t* out, size_t out_capacity);
    void decompress(std::istream& in, size_t n, std::ostream& out);
  }
}
