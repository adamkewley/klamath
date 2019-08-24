#pragma once

#include <iosfwd>

#include "src/formats/pro.hpp"

namespace klmth {
  namespace pro {
    Header parse_header(std::istream& in);
    WallData parse_wall_data(std::istream& in);
    ItemData parse_item_data(std::istream& in);
    SceneryData parse_scenery_data(std::istream& in);
  }
}
