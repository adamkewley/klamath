#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

namespace dat2 {
  struct Sections {      
    uint32_t data_section_size;
    uint32_t num_files;
    uint32_t tree_offset;
    uint32_t tree_size;
    uint32_t file_size;
  };

  struct TreeEntry {
    std::string filename;
    bool is_compressed;
    uint32_t decompressed_size;
    uint32_t packed_size;
    uint32_t offset;
  };
}
