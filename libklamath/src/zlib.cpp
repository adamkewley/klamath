#include "klamath/zlib.hpp"

#include <stdexcept>
#include <iostream>
#include <array>

#include "vendor/miniz/miniz.h"


namespace {
  struct InflateStream : public z_stream {
    InflateStream(const uint8_t* _next_in,
		  size_t _avail_in,
		  uint8_t* _next_out,
		  size_t _avail_out) {
      zalloc = Z_NULL;
      zfree = Z_NULL;
      opaque = Z_NULL;
      next_in = _next_in;
      avail_in = _avail_in;
      next_out = _next_out;
      avail_out = _avail_out;

      switch(::inflateInit(this)) {
      case Z_OK:
	break;
      default:
	throw std::runtime_error("could not initialize inflate context");
      }
    }

    InflateStream(const InflateStream& other) = delete;

    int inflate(int flush) {
      switch (::inflate(this, flush)) {
      case Z_STREAM_END:
	return Z_STREAM_END;
      case Z_OK:
	return Z_OK;
      case Z_DATA_ERROR:
	throw std::runtime_error("data error");
      case Z_STREAM_ERROR:
	throw std::runtime_error("stream error");
      case Z_MEM_ERROR:
	throw std::runtime_error("mem error");
      case Z_BUF_ERROR:
	throw std::runtime_error("buffer error");
      default:
	throw std::runtime_error("error inflating data");
      }
    }

    ~InflateStream() {
      inflateEnd(this);
    }
  };
}


klmth::zlib::BufferDecompressor::BufferDecompressor() {
}

void klmth::zlib::BufferDecompressor::decompress_in_one_step(const uint8_t* in,
							     size_t in_size,
							     uint8_t* out,
							     size_t out_capacity) {
  InflateStream s(in, in_size, out, out_capacity);

  switch (s.inflate(Z_FINISH)) {
  case Z_STREAM_END:
    return;  // all data inflated into outbuf
  case Z_OK:
    throw std::runtime_error("could not decompress data in one step");
  }
}


klmth::zlib::StreamDecompressor::StreamDecompressor() {
}


void klmth::zlib::StreamDecompressor::decompress(std::istream& in,
						 size_t n,
						 std::ostream& out) {
  std::array<uint8_t, 1 << 16> inbuf;
  std::array<uint8_t, 1 << 16> outbuf;
  InflateStream s(inbuf.data(), 0, outbuf.data(), outbuf.size());
  
  while (n != 0) {
    if (s.avail_in == 0) {
      in.read(reinterpret_cast<char*>(inbuf.data()), std::min(n, inbuf.size()));
      s.next_in = inbuf.data();
      
      if (in.gcount() > 0) {
	s.avail_in = in.gcount();
	n -= in.gcount();
      } else {
	return;
      }
    }

    int inflate_ret = s.inflate(Z_NO_FLUSH);
    size_t n_out = outbuf.size() - s.avail_out;
    
    out.write(reinterpret_cast<char*>(outbuf.data()), n_out);  // TODO: check errs
    s.next_out = outbuf.data();
    s.avail_out = outbuf.size();

    switch (inflate_ret) {
    case Z_OK:
      break;
    case Z_STREAM_END:
      return;
    }
  }
}
