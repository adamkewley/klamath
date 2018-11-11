#ifndef PROJECT_DATA_READERS_H
#define PROJECT_DATA_READERS_H

#include <cstdint>

namespace klamath {
    static uint32_t read_le_u32(const void* ptr) {
        return *((uint32_t*)ptr);
    }

    static uint32_t read_be_u32(const void* ptr) {
        auto* ptrr = (uint8_t*)ptr;
        return (ptrr[0] << 24) | (ptrr[1] << 16) | (ptrr[2] << 8) | (ptrr[3] << 0);
    }

    static uint16_t read_be_u16(const void* ptr) {
        auto* ptrr = (uint8_t*)ptr;
        return (ptrr[0] << 8) | (ptrr[1] << 0);
    }
}

#endif //PROJECT_DATA_READERS_H
