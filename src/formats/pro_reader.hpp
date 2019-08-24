#pragma once

#include <iosfwd>

#include "src/formats/pro.hpp"

namespace klmth {
  namespace pro {
    Header read_header(std::istream& in);
    WallData read_wall_data(std::istream& in);
    ItemData read_item_data(std::istream& in);
    SceneryData read_scenery_data(std::istream& in);
  }
}
