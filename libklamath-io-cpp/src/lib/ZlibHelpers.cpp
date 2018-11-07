#include "ZlibHelpers.h"

#include <stdexcept>

#include <zlib.h>

namespace klamath {
    namespace {
        const size_t BUF_SIZE = 262144;

        void copy_bytes(std::vector<uint8_t>& v, size_t offset, const uint8_t *data, size_t n) {
            v.insert(v.begin() + offset, data, data + n);
        }
    }

    std::vector<uint8_t> ZlibHelpers::inflate_all(const uint8_t *data, size_t n, size_t size_hint) {
        size_t bytes_written_to_ret = 0;
        std::vector<uint8_t> ret;
        ret.reserve(size_hint);

        uint8_t zlib_buf[BUF_SIZE];

        z_stream zlib_stream;
        zlib_stream.zalloc = Z_NULL;
        zlib_stream.zfree = Z_NULL;
        zlib_stream.opaque = Z_NULL;
        zlib_stream.avail_in = static_cast<uInt>(n);
        zlib_stream.next_in = const_cast<Bytef *>(data);
        zlib_stream.avail_out = sizeof(zlib_buf);
        zlib_stream.next_out = zlib_buf;

        if (inflateInit(&zlib_stream) != Z_OK) {
            throw std::runtime_error("could not initialize a zlib stream");
        }

        while (zlib_stream.avail_in > 0) {
            int inflate_ret = inflate(&zlib_stream, Z_FINISH);
            size_t bytes_written_to_zlib_buf = sizeof(zlib_buf) - zlib_stream.avail_out;

            switch (inflate_ret) {
                case Z_OK:
                case Z_STREAM_END:
                    copy_bytes(ret, bytes_written_to_ret, zlib_stream.next_out - bytes_written_to_zlib_buf, bytes_written_to_zlib_buf);
                    inflateEnd(&zlib_stream);
                    break;
                case Z_STREAM_ERROR:
                    inflateEnd(&zlib_stream);
                    throw std::runtime_error("error in zlib stream");
                case Z_BUF_ERROR:
                    copy_bytes(ret, bytes_written_to_ret, zlib_stream.next_out - bytes_written_to_zlib_buf, bytes_written_to_zlib_buf);
                    bytes_written_to_ret += bytes_written_to_zlib_buf;
                    zlib_stream.next_out = zlib_buf;
                    zlib_stream.avail_out = sizeof(zlib_buf);
                    break;
                default:
                    inflateEnd(&zlib_stream);
                    throw std::runtime_error("Unknown inflate return value");
            }
        }

        return std::move(ret);
    }
}