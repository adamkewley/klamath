#include <iostream>

#include "../../tmp/Dat2FileViewer.h"

#include "Dat2Extract.h"
#include "AafPrintGlyphs.h"
#include "FrmPrintHeader.h"
#include "PalDump.h"
#include "FrmDumpFirstFrame.h"

using namespace klamath;

int main(int argc, const char * argv[]) {
    if (argc <= 1) {
        std::cerr << "provide a command" << std::endl;
    } else {
        std::string arg1(argv[1]);

        int sub_argc = argc - 2;
        const char** sub_argv = argv + 2;

        if (arg1 == "aaf-print-glyphs") {
            return aaf_print_glyphs_main(sub_argc, sub_argv);
        } else if (arg1 == "frm-print-header") {
            return frm_print_header_main(sub_argc, sub_argv);
        } else if (arg1 == "pal-dump") {
            return pal_dump_main(sub_argc, sub_argv);
        } else if (arg1 == "frm-dump-first-frame") {
            return frm_dump_first_frame_main(sub_argc, sub_argv);
        } else {
                std::cerr << "unknown command" << std::endl;
                return 1;
        }
    }
}