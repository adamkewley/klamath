#include "Dat2Extract.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <cstring>
#include <algorithm>
#include <vector>

#include <utils/MemoryStream.h>
#include <utils/RandomAccessStream.h>
#include <dat2/Dat2Parser.h>
#include "lib/Mmap.h"
#include "ZlibHelpers.h"
#include "lib/ZlibStream.h"

namespace {
    using namespace klamath;

    bool file_exists(const std::string& filename) {
        struct stat st = {0};
        return stat(filename.c_str(), &st) != -1;
    }

    std::vector<std::string> split(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    void dat2_extract_mkdir(const std::string& filename) {
        std::vector<std::string> els = split(filename, '\\');
        size_t dir_els = els.size() - 1;
        std::string cur;
        for (size_t i = 0; i < dir_els; i++) {
            cur += els[i] + "/";
            if (!file_exists(cur)) {
                if (mkdir(cur.c_str(), 0777) == -1) {
                    throw std::runtime_error(cur + ": " + strerror(errno));
                }
            }
        }
    }

    void dat2_extract_entry(const std::string& entry_name, ZlibStream& s) {
        dat2_extract_mkdir(entry_name);

        std::string unix_name = entry_name;
        std::replace(unix_name.begin(), unix_name.end(), '\\', '/');

        std::ofstream out;
        out.open(unix_name, std::ios::out | std::ios::binary);

        uint8_t outbuf[256];
        size_t n = 0;
        while ((n = s.read(outbuf, 256)) != 0) {
            out.write((char *)outbuf, n);
        }

        out.close();
    }

    void dat2_extract(MemoryStream& s) {
        Dat2TopLevelHeaders h = dat2_parse_top_level_headers(s);

        std::vector<Dat2TreeEntry> entries;
        entries.reserve(h.num_files);

        for (size_t i = 0; i < h.num_files; ++i) {
            entries.push_back(dat2_parse_tree_entry(s));
        }

        for (const Dat2TreeEntry& e : entries) {
            MemoryStream entry_stream = s.substream(e.offset, e.packed_size);
            ZlibStream zls(entry_stream);
            dat2_extract_entry(e.filename, zls);
        }
    }
}

int klamath::dat2_extract_main(int argc, const char **argv) {
    if (argc == 0) {
        std::cerr << "provide dat file as arg" << std::endl;
        return 1;
    } else {
        std::string filename(argv[0]);
        Mmap m = Mmap::from_file(filename);
        MemoryStream s(m.get(), m.size());
        dat2_extract(s);

        return 0;
    }
}
