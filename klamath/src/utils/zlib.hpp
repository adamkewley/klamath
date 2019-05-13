#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>

#include "third_party/nonstd/span.hpp"

namespace klmth {
  namespace zlib {
    void decompress(nonstd::span<const uint8_t> in,
                    nonstd::span<uint8_t> out);
    
    void decompress(std::istream& in, size_t n, std::ostream& out);
  }
}
