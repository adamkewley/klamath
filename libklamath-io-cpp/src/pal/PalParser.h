#ifndef PROJECT_PALPARSER_H
#define PROJECT_PALPARSER_H

#include <cstddef>
#include "PalFile.h"

namespace klamath {

    PalFile pal_parse(const uint8_t* data, size_t size);
}

#endif //PROJECT_PALPARSER_H
