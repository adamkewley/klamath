#include "FrmPrintHeader.h"

#include <iostream>

#include <lib/Mmap.h>
#include <frm/FrmParser.h>

int klamath::frm_print_header_main(int argc, const char **argv) {
    if (argc == 0) {
        std::cerr << "provide FRM file as arg" << std::endl;
        return 1;
    } else {
        std::string filename(argv[0]);
        Mmap m = Mmap::from_file(filename);
        FrmHeader h = frm_parse_header(m.get(), m.size());

        std::cout << "version: " << h.version_number << std::endl;
        std::cout << "fps: " << h.fps << std::endl;
        std::cout << "action frame: " << h.fps << std::endl;
        std::cout << "frames per direction: " << h.frames_per_direction << std::endl;
        // TODO: print shifts etc.
        std::cout << "size of frame data: " << h.size_of_frame_data << std::endl;
        return 0;
    }
}
