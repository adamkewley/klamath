#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

namespace klamath {
    class ZlibHelpers {
    public:
        static std::vector<uint8_t> inflate_all(const uint8_t *data, size_t n, size_t size_hint);
    };
}
