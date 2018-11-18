#include "Dat2Extract.h"

#include <iostream>
#include <fstream>

#include <dat2/Dat2FileViewer.h>
#include <sstream>
#include <sys/stat.h>
#include <cstring>
#include <algorithm>

namespace {
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

    void dat2_write_entry(const std::string& filename, const std::vector<uint8_t>& data) {
        std::string platform_filename = filename;
        std::replace(platform_filename.begin(), platform_filename.end(), '\\', '/');

        std::ofstream out;
        out.open(platform_filename, std::ios::out | std::ios::binary);
        out.write((const char*)data.data(), data.size());
        out.close();
    }

    void dat2_extract_entry(const std::string& entry_name, const std::vector<uint8_t>& data) {
        dat2_extract_mkdir(entry_name);
        dat2_write_entry(entry_name, data);
    }
}

int klamath::dat2_extract_main(int argc, const char **argv) {
    if (argc == 0) {
        std::cerr << "provide dat file as arg" << std::endl;
        return 1;
    } else {
        std::string filename(argv[0]);

        Dat2FileViewer d2f = Dat2FileViewer::from_file(filename);
        for (const auto& name : d2f.entry_names()) {
            tl::optional<std::vector<uint8_t>> maybe_data = d2f.read_entry(name);
            if (maybe_data.has_value()) {
                std::vector<uint8_t> data = d2f.read_entry(name).value();
                dat2_extract_entry(name, data);
            } else {
                std::cerr << std::string("missing") + name << std::endl;
            }
        }

        return 0;
    }
}
