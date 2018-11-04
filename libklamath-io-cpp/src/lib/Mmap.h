#ifndef PROJECT_MMAPED_DATA_H
#define PROJECT_MMAPED_DATA_H

#include <string>

namespace klamath {
    class Mmap {
    public:
        static Mmap from_file(const std::string& path);

        Mmap(const Mmap&) = delete;  // We don't want accidental copies
        Mmap(Mmap&&) noexcept;
        ~Mmap();

        const uint8_t* get() const;
        const size_t size() const;

    private:
        Mmap(void* mmap_ptr, size_t filesize);
        void* _mmap_ptr;
        size_t _filesize;
    };
}

#endif //PROJECT_MMAPED_DATA_H
