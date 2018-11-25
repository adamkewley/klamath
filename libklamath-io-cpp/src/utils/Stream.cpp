#include <stdexcept>
#include "Stream.h"

uint32_t klamath::Stream::read_little_endian_u32() {
    uint32_t ret = 0;
    read((uint8_t*)&ret, 4);  // assumes CPU is little endian
    return ret;
}

uint32_t klamath::Stream::read_big_endian_u32() {
    uint32_t ret = 0;
    ret |= (uint32_t)read_u8() << 24;
    ret |= (uint32_t)read_u8() << 16;
    ret |= (uint32_t)read_u8() << 8;
    ret |= (uint32_t)read_u8();
    return ret;
}

uint16_t klamath::Stream::read_big_endian_u16() {
    uint16_t ret = 0;
    ret |= (uint16_t)read_u8() << 8;
    ret |= (uint16_t)read_u8();
    return ret;
}

int32_t klamath::Stream::read_big_endian_i32() {
    // assumes CPU is using same signing method as data (2's complement)
    return read_big_endian_u32();
}

int16_t klamath::Stream::read_big_endian_i16() {
    // assumes CPU is using same signing method as data (2's complement)
    return read_big_endian_u16();
}

std::vector<uint8_t> klamath::Stream::read_n_bytes(size_t n) {
    std::vector<uint8_t> ret;
    ret.reserve(n);

    read(&ret.front(), n);

    return ret;
}
