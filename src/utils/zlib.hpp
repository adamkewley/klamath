#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>

namespace klmth {
  namespace zlib {
    void decompress(const uint8_t* in_buf,
                    size_t in_sz,
                    uint8_t* out_buf,
                    size_t out_sz);
    
    void decompress(std::istream& in,
                    size_t n,
                    std::ostream& out);
  }
}
