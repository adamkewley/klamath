#ifndef PROJECT_STREAM_H
#define PROJECT_STREAM_H

#include <vector>
#include <cstdint>
#include <cstddef>

namespace klamath {
    class Stream {
    public:
        Stream(const uint8_t* data, size_t len);

        uint8_t read_u8();
        uint32_t read_little_endian_u32();
        uint32_t read_big_endian_u32();
        uint16_t read_big_endian_u16();
        std::vector<uint8_t> read_n_bytes(size_t n);

        size_t remaining() const;

    private:
        const uint8_t* _data;
        const size_t  _len;
        size_t offset = 0;

        void assert_has_remaining(size_t num_bytes) const;
        const uint8_t* read_then_advance(size_t num_bytes);
    };
}

#endif //PROJECT_STREAM_H
