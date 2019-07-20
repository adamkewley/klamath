#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <unordered_map>
#include <sys/stat.h>
#include <sys/types.h>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"
#include "src/formats/lst.hpp"
#include "src/formats/lst_reader.hpp"
#include "src/formats/frm.hpp"
#include "src/formats/frm_reader.hpp"
#include "src/formats/pal.hpp"
#include "src/formats/pal_reader.hpp"
#include "src/utils/sdl.hpp"

using namespace klmth;
using std::this_thread::sleep_for;
using namespace std::literals::chrono_literals;
using sdl::StaticTexture;

namespace {
  std::string upcase(const std::string& s) {
    auto xform = [](unsigned char c){
      return std::toupper(c);
    };
    
    std::string copy = s;
    std::transform(copy.begin(), copy.end(), copy.begin(), xform);
    return copy;
  }

  std::string downcase(const std::string& s) {
    auto xform = [](unsigned char c){
      return std::tolower(c);
    };
    
    std::string copy = s;
    std::transform(copy.begin(), copy.end(), copy.begin(), xform);
    return copy;
  }

  bool file_exists(const std::string& filename) {
    struct stat st = {0};
    return stat(filename.c_str(), &st) != -1;
  }

  std::ifstream open_file(const std::string& pth) {
    std::ifstream f{pth, std::ios::in | std::ios::binary};

    if (f.good()) {
      f.exceptions(std::ifstream::badbit);
      return f;
    } else {
      std::stringstream err;
      err << pth << ": " << strerror(errno);
      throw std::runtime_error{err.str()};
    }
  }

  pal::File read_pal(const std::string& pth) {
    std::ifstream pal_file = open_file(pth);
    return pal::parse(pal_file);
  }

  lst::File read_tiles(const std::string& tiles_dir_pth) {
    std::ifstream tiles_file = open_file(tiles_dir_pth + "/TILES.LST");
    return lst::parse_file(tiles_file);
  }

  frm::File read_frm(const std::string& pth) {
    std::ifstream fd = open_file(pth);
    return frm::read_file(fd);
  }
  
  struct NamedStrm {
    std::istream& strm;
    std::string name;
  };

  using TileCache = std::unordered_map<map::TileId, StaticTexture>;

  struct AppState {
    pal::File pal;
    std::string tiles_dir;
    lst::File tile_lut;
    sdl::Context context;
    sdl::Window window;
    TileCache tile_cache;
  };

  StaticTexture load_tile(AppState& st, std::string& pth) {
    frm::File file = read_frm(pth);

    // assumption: tiles only *really* have one orientation.
    const frm::Animation& anim = file.animation(frm::north_east);

    if (anim.frames().empty()) {
      std::stringstream msg;
      msg << pth << ": tile contains no frames";
      throw std::runtime_error{msg.str()};
    }

    return st.window.texture(st.pal, anim.frames().front());
  }

  void populate_tile_cache(AppState& st, uint16_t tileid) {
    if (tileid == map::null_tileid) {
      return;
    }
    
    if (st.tile_cache.find(tileid) != st.tile_cache.end()) {
      return;  // already in cache
    }

    if (tileid >= st.tile_lut.size()) {
      std::stringstream msg;
      msg << tileid << ": tile ID in map file is higher than number of tiles in lookup (usually, TILES.LST), which contains " << st.tile_lut.size() << " entries";
      throw std::runtime_error{msg.str()};
    }

    const std::string& tile_name = st.tile_lut[tileid];
    
    // todo: case-insensitive pathing
    std::string tile_path =
      file_exists(st.tiles_dir + upcase(tile_name)) ?
      st.tiles_dir + upcase(tile_name) :
      file_exists(st.tiles_dir + downcase(tile_name)) ?
      st.tiles_dir + downcase(tile_name) :
      st.tiles_dir + tile_name;
      
    st.tile_cache.emplace(tileid, load_tile(st, tile_path));
  }

  void populate_tile_cache(AppState& st, const map::Tile& tile) {
    populate_tile_cache(st, tile.floor_id);    
  }

  void populate_tile_cache(AppState& st, const map::Tiles& tiles) {
    for (map::Tile tile : tiles) {
      populate_tile_cache(st, tile);
    }
  }

  void render_frame(AppState& st, const map::Tiles& tiles, sdl::Point view_pos) {
    // Mapping tile coordinates to screen coordinates:
    //
    // Fallout uses trimetric projection for tiles (note: NOT
    // isometric or dimetric). Each tile texture is 80 px across
    // and 36 px high, but the visual content of each texture is a
    // parallelogram in the texture.
    //
    // You can imagine the shape of the tile by rotating a rectangle
    // ~60 deg (this isn't the actual angle) then looking at it from a
    // slanted perspective. The resulting top and bottom corners don't
    // line up vertically, so tesselating the tiles in the screen's
    // coordinate system isn't a simple matter of (e.g.)  0.5*width +
    // 0.5*height.
    constexpr sdl::Dimensions tile_dimensions{map::Tile::width, map::Tile::height};
    
    for (auto row = 0U; row < map::rows; ++row) {
      
      auto row_beg = map::cols * row;
      sdl::Point row_offset{static_cast<int>(32*row), static_cast<int>(24*row)};
      sdl::Point row_origin_in_view = row_offset - view_pos;
      
      for (auto col = 0U; col < map::cols; ++col) {
        const map::Tile& tile = tiles.at(row_beg + col);

        // edge case: null tile isn't renderable
        if (tile.floor_id == map::null_tileid) {
          continue;
        }

        sdl::Point col_offset{static_cast<int>(48*(map::cols - col)), static_cast<int>(12*col)};
        sdl::Point pos_in_view = row_origin_in_view + col_offset;

        StaticTexture& texture = st.tile_cache.at(tile.floor_id);
        sdl::Rect dest{pos_in_view, tile_dimensions};
        st.window.render_texture(texture, dest);
      }
    }
  }

  void show_elevation(AppState& st, const map::Tiles& tiles) {
    populate_tile_cache(st, tiles);

    for (int i = 0; i < 360; ++i) {
      sdl::Point screen_pos{25*i, 10*i};
      
      st.window.render_clear();
      render_frame(st, tiles, screen_pos);
      st.window.render_present();
      sleep_for(25ms);
    }
  }

  void show_lowest_map_elevation(AppState& st, NamedStrm& in) {
    map::File f = map::parse_file(in.strm);

    std::unique_ptr<map::Tiles> null_tiles;
    std::unique_ptr<map::Tiles>& lowest =
      f.low_elevation_tiles ?
      f.low_elevation_tiles :
      f.mid_elevation_tiles ?
      f.mid_elevation_tiles :
      f.high_elevation_tiles ?
      f.high_elevation_tiles :
      null_tiles;

    if (lowest == nullptr) {
      throw std::runtime_error{"no tiles at any elevation"};
    } else {
      show_elevation(st, *lowest);
    }    
  }

  void show_map_strm(AppState& st, NamedStrm in) {
    try {
      show_lowest_map_elevation(st, in);
    } catch (const std::exception& ex) {
      std::stringstream msg;
      msg << in.name << ": " << ex.what();
      throw std::runtime_error{msg.str()};
    }
  }

  void show_map_pths(AppState& st, const std::vector<std::string>& map_pths) {
    for (const std::string& map_pth : map_pths) {
      if (map_pth == "-") {
        show_map_strm(st, { std::cin, "stdin" });
      } else {
        std::ifstream in = open_file(map_pth);
        show_map_strm(st, { in, map_pth });
      }
    }
  }
}

int cmd_mapshow(int argc, char** argv) {
  std::string pal_pth;
  std::string tiles_dir_pth;
  std::vector<std::string> map_pths;

  CLI::App app{"show MAP files visually"};
  app.add_option("pal_pth", pal_pth, "path to a PAL file")->required();
  app.add_option("tiles_dir", tiles_dir_pth, "path to tiles dir (e.g. out/art/tiles). Must contain a TILES.LST file, which lists all tiles FRMs that are used in the map (by indexed ID)")->required();
  app.add_option("map_file", map_pths, "path to map file(s). '-' is interpreted as stdin. Supplying no paths will cause application to read MAP from stdin");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& ex) {
    return app.exit(ex);
  }

  try {
    sdl::Context c;
    sdl::Window w = c.create_window({ 1000, 1000 });
    AppState st{ read_pal(pal_pth), tiles_dir_pth, read_tiles(tiles_dir_pth), std::move(c), std::move(w), {} };
    if (map_pths.empty()) {
      show_map_strm(st, { std::cin, "stdin" });
    } else {
      show_map_pths(st, map_pths);
    }
    return 0;
  } catch (const std::ios_base::failure& ex) {
    std::cerr << "mapshow: " << ex.what() << ": " << strerror(errno) << std::endl;
    return 1;
  } catch (const std::exception& ex) {
    std::cerr << "mapshow: " << ex.what() << std::endl;
    return 1;
  }
}
