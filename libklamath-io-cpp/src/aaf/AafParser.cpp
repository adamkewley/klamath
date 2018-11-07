#include "AafParser.h"

namespace klamath {
    AafFile AafParser::parse(uint8_t const *data, size_t data_avail) {
        return AafFile(0, 0, 0, 0, std::vector<AafGlyph>());
    }
}
