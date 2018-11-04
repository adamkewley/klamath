#include "Mmap.h"

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <stdexcept>
#include <cstring>
#include <unistd.h>

namespace klamath {
    Mmap Mmap::from_file(const std::string &path) {
        int fd = open(path.c_str(), O_RDONLY);
        if (fd == -1) {
            throw std::runtime_error(path + ": cannot open: " + strerror(errno));
        }

        struct stat buf = {0};
        if (fstat(fd, &buf) == -1) {
            close(fd);
            throw std::runtime_error(path + ": cannot stat: " + strerror(errno));
        }

        void* mmap_ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mmap_ptr == MAP_FAILED) {
            close(fd);
            throw std::runtime_error(path + ": cannot mmap: " + strerror(errno));
        }

        return Mmap(mmap_ptr, buf.st_size);
    }


    Mmap::Mmap(void *mmap_ptr, size_t filesize): _mmap_ptr(mmap_ptr), _filesize(filesize) {
    }

    Mmap::Mmap(Mmap && other) noexcept :  _mmap_ptr(other._mmap_ptr), _filesize(other._filesize) {
        other._mmap_ptr = nullptr;
        other._filesize = 0;
    }

    Mmap::~Mmap() {
        munmap(_mmap_ptr, _filesize);
    }


    const uint8_t* Mmap::get() const {
        return static_cast<const uint8_t *>(_mmap_ptr);
    }

    const size_t Mmap::size() const {
        return _filesize;
    }
}
