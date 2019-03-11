#pragma once

#include <cstdint>
#include <cstddef>
#include <iosfwd>


namespace klmth {
  namespace zlib {
    class BufferDecompressor {
    public:
      BufferDecompressor();
      BufferDecompressor(const BufferDecompressor& other) = delete;

      void decompress_in_one_step(const uint8_t* in,
				  size_t in_size,
				  uint8_t* out,
				  size_t out_size);
    };

    class StreamDecompressor {
    public:
      StreamDecompressor();
      StreamDecompressor(const StreamDecompressor& other) = delete;

      void decompress(std::istream& in,
		      size_t n,
		      std::ostream& out);
    };
  }
}
