#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

namespace klamath {
    class Stream {
    public:
        virtual size_t read(uint8_t* buf, size_t count) = 0;
        virtual uint8_t read_u8() = 0;

        uint32_t read_little_endian_u32();

        uint32_t read_big_endian_u32();

        uint16_t read_big_endian_u16();

        int32_t read_big_endian_i32();

        int16_t read_big_endian_i16();

        std::vector<uint8_t> read_n_bytes(size_t n);
    };
}
