#pragma once

#include <cstddef>
#include <cstdint>

#include "../utils/Stream.h"
#include "FrmHeader.h"
#include "FrmFrame.h"

namespace klamath {
    FrmHeader frm_parse_header(Stream& s);
    FrmFrame frm_parse_frame(Stream& s);
}
