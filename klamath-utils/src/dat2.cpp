#include "dat2.h"

#include <sys/mman.h>
#include <iostream>

#include <dat2/Dat2FileViewer.h>
#include <aaf/AafParser.h>
#include <pal/PalParser.h>

namespace klamath {
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


    void print_font(Dat2FileViewer& d2f) {
        auto data = d2f.read_entry(R"(FONT1.AAF)");
        AafFile f = aaf_parse(data.value().data(), data.value().size());
        for (uint8_t c = 97; c <123; ++c) {
            print_glyph(f.glyphs[c]);
        }
    }

    void parse_pal(Dat2FileViewer& d2f) {
        auto pal_data = d2f.read_entry("art\\intrface\\EG_MOD01.PAL");
        PalFile pf = pal_parse(pal_data.value().data(), pal_data.value().size());
        for (const RgbTriple& color : pf.palette) {
            printf("%u %u %u\n", color.r, color.g, color.b);
        }
    }

    int dat2_main(int argc, const char **argv) {
        if (argc == 0) {
            std::cerr << "provide dat file as arg" << std::endl;
            return 1;
        } else {
            std::string filename(argv[0]);
            Dat2FileViewer d2f = Dat2FileViewer::from_file(filename);
            for (const auto& name : d2f.entry_names()) {
                std::cout << name << std::endl;
            }

            print_font(d2f);
            parse_pal(d2f);

            return 0;
        }
    }
}