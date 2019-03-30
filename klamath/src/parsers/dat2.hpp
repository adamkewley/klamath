#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <iosfwd>

namespace klmth {
  namespace dat2 {
    struct Sections {
      Sections(uint32_t _data_section_size,
	       uint32_t _num_files,
	       uint32_t _tree_offset,
	       uint32_t _tree_size,
	       uint32_t _file_size);
      
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

    Sections read_sections(std::istream& dat2_in);
    void read_tree_entry(std::istream& entry_in, TreeEntry& out);
  }
}
