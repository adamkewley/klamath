#ifndef PROJECT_AAFPARSER_H
#define PROJECT_AAFPARSER_H

#include <cstddef>
#include <cstdint>

#include "AafFile.h"

namespace klamath {

    AafFile aaf_parse(const uint8_t* data, size_t size);
}

#endif //PROJECT_AAFPARSER_H
