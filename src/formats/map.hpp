#pragma once

#include <cstdint>
#include <memory>

#include "src/utils/geometry.hpp"
#include "src/formats/frm.hpp"

namespace klmth {
  namespace map {

    enum Version {
      fallout_1 = 19,
      fallout_2 = 20
    };

    enum class Elevation {
      low,
      med,
      high,
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

    using TileId = uint16_t;
    constexpr TileId null_tileid = 1;

    struct Tile {
      TileId roof_id;
      TileId floor_id;

      static constexpr uint16_t width = 80;
      static constexpr uint16_t height = 36;
    };

    constexpr unsigned rows = 100;
    constexpr unsigned cols = 100;
    constexpr unsigned tiles_per_elevation = rows * cols;

    using Tiles = std::array<Tile, tiles_per_elevation>;

    enum class ScriptType {
      system,
      spatial,
      timer,
      item,
      critter,
    };

    char const * str(ScriptType type);

    class Pid {
    public:
      Pid(uint32_t val);
      ScriptType type() const noexcept;
    private:
      ScriptType _type;
    };

    struct Script {
      Pid pid;
    };

    struct File {
      Header header;
      std::vector<int32_t> global_vars;
      std::vector<int32_t> local_vars;
      std::unique_ptr<Tiles> low_elevation_tiles;
      std::unique_ptr<Tiles> mid_elevation_tiles;
      std::unique_ptr<Tiles> high_elevation_tiles;
      std::vector<Script> scripts;

      const std::unique_ptr<Tiles>& tiles(Elevation e) const;
      std::vector<Elevation> elevations() const noexcept;
    };
  }
}
