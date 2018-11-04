#include <iostream>
#include <sys/mman.h>
#include <dat2/dat2_file_viewer.h>

#include "dat2.h"

namespace klamath {
    int dat2_main(int argc, const char **argv) {
        if (argc == 0) {
            std::cerr << "provide dat file as arg" << std::endl;
            return 1;
        } else {
            std::string filename(argv[0]);
            Dat2FileViewer d2f = Dat2FileViewer::from_file(filename);
            return 0;
        }
    }
}