#pragma once

#include <cstdint>

#include "src/formats/frm.hpp"

namespace klmth {
  namespace map {
    
    enum Version {
      fallout_1 = 19,
      fallout_2 = 20
    };
    
    enum Elevation {
      low = 0,
      med = 1,
      high = 3,
    };

    struct PlayerDefaults {
      int32_t pos;
      Elevation elevation;
      frm::Orientation orientation;
    };
    
    struct Header {
      Version version;
      std::string filename;
      PlayerDefaults player_defaults;
      int32_t num_local_vars;
      int32_t script_id;
      bool is_savegame_map;
      bool has_low_elevation;
      bool has_med_elevation;
      bool has_high_elevation;
      int32_t num_global_vars;
      int32_t map_id;
      uint32_t timestamp;
    };

    struct File {
      Header header;
      std::vector<int32_t> global_vars;
    };
  }
}
