#ifndef PROJECT_FRMPARSER_H
#define PROJECT_FRMPARSER_H

#include <cstddef>
#include <cstdint>

#include "FrmHeader.h"
#include "FrmFrame.h"

namespace klamath {
    FrmHeader frm_parse_header(const uint8_t* data, size_t size);
    FrmFrame frm_parse_frame(const uint8_t* data, size_t size);
}

#endif //PROJECT_FRMPARSER_H
