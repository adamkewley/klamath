#pragma once

#include <cstdint>

#include "utils/RandomAccessStream.h"

namespace klamath {
    class MemoryStream : public RandomAccessStream {
    public:
        MemoryStream(const uint8_t* data, size_t len);

        uint8_t read_u8() override;

        size_t read(uint8_t *buf, size_t count) override;

        void seekg(size_t pos) override;

        MemoryStream substream(size_t offset, size_t len);

        size_t remaining() override;

    private:
        const uint8_t* _data;
        const size_t  _len;
        size_t _pos = 0;

        void assert_has_remaining(size_t num_bytes) const;
    };
}
