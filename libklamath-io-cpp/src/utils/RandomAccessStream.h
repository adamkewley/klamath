#pragma once

#include "Stream.h"

namespace klamath {
    class RandomAccessStream : public Stream {
    public:
        virtual void seekg(size_t pos) = 0;
        virtual size_t remaining() = 0;
    };
}
