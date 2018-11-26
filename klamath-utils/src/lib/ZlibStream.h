#pragma once

#include <utils/RandomAccessStream.h>

namespace klamath {
    const size_t ZLIB_BUFFER_SIZE = 262144;

    class ZlibStream : public Stream {
    public:
        explicit ZlibStream(Stream& source);

        uint8_t read_u8() override;

        size_t read(uint8_t *buf, size_t count) override;

    private:
        Stream& _source;
        uint8_t _buf[ZLIB_BUFFER_SIZE];
        size_t _pos;
        size_t _remaining;
        bool _finished;
    };
}

