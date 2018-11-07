#ifndef PROJECT_AAF_PARSER_H
#define PROJECT_AAF_PARSER_H

#include <cstddef>
#include <cstdint>
#include "AafFile.h"

namespace klamath {
    class AafParser {
    public:
        AafFile static parse(uint8_t const * data, size_t data_avail);
    };
}

#endif //PROJECT_AAF_PARSER_H
