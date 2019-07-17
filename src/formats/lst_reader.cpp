#include "src/formats/lst_reader.hpp"

#include <array>
#include <istream>

std::vector<std::string> klmth::lst::parse_file(std::istream& in) {
  // Doing it this way, rather than char-by-char, reduces virtual call
  // overhead and prevents overallocating the comment string only to
  // cull it later.
  std::array<char, 1<<16> buf;
  std::string cur;
  bool skip = false;
  std::vector<std::string> ret;
  
  while (!in.eof()) {
    in.read(buf.data(), buf.size());
    auto avail = in.gcount();
    
    for (int i = 0; i < avail; ++i) {
      char c = buf[i];
      switch (c) {
      case '\r':
        continue;
      case '\n':
        ret.emplace_back(std::move(cur));
        cur.clear();
        skip = false;
        continue;
      case ';':
        skip = true;
        continue;
      }

      if (!skip) {
        cur += c;
      }
    }
  }

  // trailing newline is not specified
  if (!cur.empty()) {
    ret.emplace_back(std::move(cur));
  }

  return ret;
}
