#include <stdexcept>
#include "stream.h"

klamath::Stream::Stream(const uint8_t *data, size_t len) : _data(data), _len(len) {}

uint8_t klamath::Stream::read_u8() {
    return *read_then_advance(1);
}

uint32_t klamath::Stream::read_little_endian_u32() {
    return *((uint32_t*)read_then_advance(4));
}

uint32_t klamath::Stream::read_big_endian_u32() {
    const uint8_t* ptrr = read_then_advance(4);
    return (ptrr[0] << 24) | (ptrr[1] << 16) | (ptrr[2] << 8) | (ptrr[3] << 0);
}

uint16_t klamath::Stream::read_big_endian_u16() {
    const uint8_t* ptrr = (uint8_t*)read_then_advance(2);
    return (ptrr[0] << 8) | (ptrr[1] << 0);
}

std::vector<uint8_t> klamath::Stream::read_n_bytes(size_t n) {
    const uint8_t* data = read_then_advance(n);

    std::vector<uint8_t> ret;
    ret.reserve(n);
    for (size_t i = 0; i < n; i++) {
        ret.push_back(data[i]);
    }

    return ret;
}

size_t klamath::Stream::remaining() const {
    return this->_len - offset;
}

void klamath::Stream::assert_has_remaining(size_t num_bytes) const {
    if (num_bytes > remaining()) {
        throw std::runtime_error("tried to read from an empty stream");
    }
}

const uint8_t *klamath::Stream::read_then_advance(size_t num_bytes) {
    assert_has_remaining(num_bytes);
    const uint8_t* ret = this->_data + this->offset;
    offset += num_bytes;
    return ret;
}
