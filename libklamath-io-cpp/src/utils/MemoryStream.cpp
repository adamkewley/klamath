#include "MemoryStream.h"

#include <stdexcept>

klamath::MemoryStream::MemoryStream(const uint8_t *data, size_t len) : _data(data), _len(len) {
}

uint8_t klamath::MemoryStream::read_u8() {
    assert_has_remaining(1);
    return *(_data + _pos++);
}

void klamath::MemoryStream::seekg(size_t pos) {
    if (pos >= _len) {
        throw std::runtime_error("tried to seek past end of stream");
    } else {
        this->_pos = pos;
    }
}

void klamath::MemoryStream::assert_has_remaining(size_t num_bytes) const {
    if (num_bytes + _pos >= _len) {
        throw std::runtime_error("tried to read from an empty stream");
    }
}

klamath::MemoryStream klamath::MemoryStream::substream(size_t offset, size_t len) {
    if (offset + len > _len) {
        throw std::runtime_error("invalid windowing");
    } else {
        return MemoryStream(_data + offset, len);
    }
}

size_t klamath::MemoryStream::read(uint8_t *buf, size_t count) {
    size_t rem = remaining();
    size_t num_read = (count < rem) ? count : rem;

    for (size_t i = 0; i < num_read; ++i) {
        *(buf++) = *(_data + _pos++);
    }

    return num_read;
}

size_t klamath::MemoryStream::remaining() {
    return _len - _pos;
}
