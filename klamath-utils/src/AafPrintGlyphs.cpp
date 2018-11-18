#include "AafPrintGlyphs.h"

#include <iostream>

#include <lib/Mmap.h>
#include <aaf/AafFile.h>
#include <aaf/AafParser.h>
#include <aaf/AafGlyph.h>

namespace {
    using namespace klamath;

    void print_glyph(const AafGlyph& g) {

        size_t len = g.opacities.size();
        for (size_t i = 0; i < len; ++i) {
            uint8_t x = g.opacities[i];
            if (i % g.width == 0) {
                std::cout << std::endl;
            }
            switch (x) {
                case 0:
                    std::cout << ' ';
                    break;
                case 1:
                    std::cout << '1';
                    break;
                case 2:
                    std::cout << '2';
                    break;
                case 3:
                    std::cout << '3';
                    break;
                case 4:
                    std::cout << '4';
                    break;
                case 5:
                    std::cout << '5';
                    break;
                case 6:
                    std::cout << '6';
                    break;
                case 7:
                    std::cout << '7';
                    break;
                case 8:
                    std::cout << '8';
                    break;
                case 9:
                    std::cout << '9';
                    break;
                default:
                    throw std::runtime_error("unknown val");
            }
        }
        std::cout << std::endl;
    }
}

int klamath::aaf_print_glyphs_main(int argc, const char **argv) {
    if (argc == 0) {
        std::cerr << "provide aaf file as arg" << std::endl;
        return 1;
    } else {
        std::string filename(argv[0]);
        Mmap m = Mmap::from_file(filename);

        AafFile f = aaf_parse(m.get(), m.size());
        for (uint8_t c = 97; c <123; ++c) {
            print_glyph(f.glyphs[c]);
        }

        return 0;
    }
}
