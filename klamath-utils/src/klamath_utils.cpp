#include <iostream>

#include <map>

#include "Dat2Extract.h"
#include "AafPrintGlyphs.h"
#include "FrmPrintHeader.h"
#include "PalDump.h"
#include "FrmDumpFirstFrame.h"

using namespace klamath;

typedef int (*clifunc)(int, const char **);

std::map<std::string, clifunc> cmds {
        { "aaf-print-glyphs", &aaf_print_glyphs_main },
        { "frm-print-header", &frm_print_header_main },
        { "pal-dump", &pal_dump_main },
        { "frm-show-first-frame", &frm_dump_first_frame_main }
};

int main(int argc, const char * argv[]) {
    if (argc <= 1) {
        std::cerr << "provide a command" << std::endl;
        for (const std::pair<const std::string, clifunc>& e : cmds) {
            std::cerr << e.first << std::endl;
        }
    } else {
        std::string arg1(argv[1]);

        if (cmds.find(arg1) != cmds.end()) {
            clifunc sub_main = cmds[arg1];
            return sub_main(argc - 2, argv + 2);
        } else {
            std::cerr << "unknown command" << std::endl;
            return 1;
        }
    }
}