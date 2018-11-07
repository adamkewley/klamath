#ifndef PROJECT_MMAP_H
#define PROJECT_MMAP_H

#include <string>

namespace klamath {
    class Mmap {
    public:
        static Mmap from_file(const std::string& path);

        Mmap(const Mmap&) = delete;  // We don't want accidental copies
        Mmap(Mmap&&) noexcept;
        ~Mmap();

        Mmap& operator=(const Mmap&) = delete;

        const uint8_t* get() const;
        const size_t size() const;

    private:
        Mmap(void* mmap_ptr, size_t filesize);
        void* _mmap_ptr;
        size_t _filesize;
    };
}

#endif //PROJECT_MMAP_H
