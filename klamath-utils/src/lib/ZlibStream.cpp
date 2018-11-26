#include "ZlibStream.h"

#include <stdexcept>

klamath::ZlibStream::ZlibStream(klamath::Stream &source) :
        _source(source),
        _pos(0),
        _remaining(0),
        _finished(false) {

}

uint8_t klamath::ZlibStream::read_u8() {
    throw std::runtime_error("reading single byte from zlib stream NYI");
}

size_t klamath::ZlibStream::read(uint8_t *buf, size_t count) {

}
