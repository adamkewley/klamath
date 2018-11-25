#include "ZlibStream.h"

#include <stdexcept>

klamath::ZlibStream::ZlibStream(klamath::Stream &source) : _source(source) {

}

uint8_t klamath::ZlibStream::read_u8() {
    return 0;
}

size_t klamath::ZlibStream::read(uint8_t *buf, size_t count) {

}
