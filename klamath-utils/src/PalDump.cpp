#include "PalDump.h"

#include <iostream>

#include "lib/Mmap.h"
#include <pal/PalRgbTriple.h>
#include <pal/PalFile.h>
#include <pal/PalParser.h>
#include <utils/MemoryStream.h>

int klamath::pal_dump_main(int argc, const char **argv) {
    if (argc == 0) {
        std::cerr << "provide pal file as arg" << std::endl;
        return 1;
    } else {
        std::string filename(argv[0]);
        Mmap m = Mmap::from_file(filename);
        MemoryStream s(m.get(), m.size());

        PalFile pf = pal_parse(s);
        for (const PalRgbTriple& color : pf.palette) {
            printf("%u %u %u\n", color.r, color.g, color.b);
        }

        return 0;
    }
}
