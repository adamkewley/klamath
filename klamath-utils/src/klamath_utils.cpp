#include <iostream>

#include "dat2/dat2_file_viewer.h"

#include "dat2.h"

int main(int argc, const char * argv[]) {
    if (argc <= 1) {
        std::cerr << "provide a command" << std::endl;
    } else {
        std::string arg1(argv[1]);
        if (arg1 == "dat2") {
            klamath::dat2_main(argc - 2, argv + 2);
        }
    }
}