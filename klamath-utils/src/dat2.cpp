#include "dat2.h"

#include <sys/mman.h>
#include <iostream>

#include <dat2/dat2_file_viewer.h>

namespace klamath {
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
            auto data = d2f.read_entry(R"(text\english\dialog\dcLara.msg)");
            std::cout << std::string((char *)data.value().data())<< std::endl;
            return 0;
        }
    }
}