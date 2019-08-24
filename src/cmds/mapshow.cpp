#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <thread>
#include <unordered_map>
#include <sys/stat.h>
#include <sys/types.h>
#include <cctype>

#include <SDL_events.h>
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
using std::unique_ptr;
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
    struct stat st{};
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
    return lst::read_file(tiles_file);
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
    sdl::Point view_pos;

    void preload(const map::Tiles& tiles) {
      for (map::Tile tile : tiles) {
        this->preload(tile);
      }
    }

    StaticTexture& tile_texture(map::TileId id) {
      return load_via_cache(id);
    }

  private:
    void preload(const map::Tile& tile) {
      if (tile.floor_id != map::null_tileid) {
        this->preload(tile.floor_id);
      }

      if (tile.roof_id != map::null_tileid) {
        this->preload(tile.roof_id);
      }
    }

    void preload(map::TileId id) {
      if (this->tile_cache.find(id) != this->tile_cache.end()) {
        return;  // already in cache
      } else {      
        this->tile_cache.emplace(id, this->load(id));
      }
    }

    StaticTexture& load_via_cache(map::TileId id) {
      if (this->tile_cache.find(id) == this->tile_cache.end()) {
        this->tile_cache.emplace(id, this->load(id));
      }
      return this->tile_cache.at(id);
    }
        
    StaticTexture load(map::TileId id) {
      if (id == map::null_tileid) {
        throw std::runtime_error{"tried to load null tile (id = 1). this is a developer error"};
      }

      if (id >= this->tile_lut.size()) {
        std::stringstream msg;
        msg << id << ": tile ID in map file is higher than number of tiles in lookup (usually, TILES.LST), which contains " << this->tile_lut.size() << " entries";
        throw std::runtime_error{msg.str()};
      }

      const std::string& tile_name = this->tile_lut[id];
    
      // todo: case-insensitive pathing
      std::string tile_path =
        file_exists(this->tiles_dir + upcase(tile_name)) ?
        this->tiles_dir + upcase(tile_name) :
        file_exists(this->tiles_dir + downcase(tile_name)) ?
        this->tiles_dir + downcase(tile_name) :
        this->tiles_dir + tile_name;

      return load(tile_path);
    }

    StaticTexture load(const std::string& pth) {
      frm::File file = read_frm(pth);
            
      // assumption: tiles only *really* have one orientation.
      const frm::Animation& anim = file.animation(frm::north_east);

      if (anim.frames().empty()) {
        std::stringstream msg;
        msg << pth << ": tile contains no frames";
        throw std::runtime_error{msg.str()};
      }

      return this->window.texture(this->pal, anim.frames().front());
    }
  };

  sdl::Point to_view_coord(unsigned row, unsigned col) {
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
    sdl::Point row_offset{static_cast<int>(32*row), static_cast<int>(24*row)};
    sdl::Point col_offset{static_cast<int>(48*(map::cols - col)), static_cast<int>(12*col)};
    return row_offset + col_offset;
  }

  void render_tile(AppState&st, sdl::Point shift, map::TileId id, unsigned row, unsigned col) {
    constexpr sdl::Dimensions tile_dimensions{map::Tile::width, map::Tile::height};

    if (id != map::null_tileid) {
      sdl::Point pos = to_view_coord(row, col) - shift;
      StaticTexture& texture = st.tile_texture(id);
      sdl::Rect dest{pos, tile_dimensions};
      st.window.render_texture(texture, dest);
    }    
  }

  void render_frame(AppState& st, const map::Tiles& tiles) {
    // todo: I just eyeballed this shift
    constexpr sdl::Point roof_shift{0, 3 * map::Tile::height};
    
    // floor
    for (auto row = 0U; row < map::rows; ++row) {
      auto row_beg = map::cols * row;      
      for (auto col = 0U; col < map::cols; ++col) {
        const map::Tile& tile = tiles.at(row_beg + col);
        render_tile(st, st.view_pos, tile.floor_id, row, col);
      }
    }

    // roof
    for (auto row = 0U; row < map::rows; ++row) {
      auto row_beg = map::cols * row;      
      for (auto col = 0U; col < map::cols; ++col) {
        const map::Tile& tile = tiles.at(row_beg + col);
        render_tile(st, st.view_pos + roof_shift, tile.roof_id, row, col);
      }
    }
  }

  void render(AppState& st, const map::File& f) {
    const std::vector<map::Elevation> elevations = f.elevations();

    if (elevations.empty()) {
      throw std::runtime_error{"no tiles at any elevation"};
    }

    size_t elevation_idx = 0;

    SDL_Event e;
    while (true) {
      const unique_ptr<map::Tiles>& tiles = f.tiles(elevations[elevation_idx]);
      st.preload(*tiles);
      st.window.render_clear();
      render_frame(st, *tiles);
      st.window.render_present();

      st.context.wait_for_event(&e);

      switch (e.type) {
      case SDL_QUIT:
        return;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_UP:
          st.view_pos += {0, -100};
          break;
        case SDLK_RIGHT:
          st.view_pos += {100, 0};
          break;
        case SDLK_DOWN:
          st.view_pos += {0, 100};
          break;
        case SDLK_LEFT:
          st.view_pos += {-100, 0};
          break;
        case SDLK_ESCAPE:
          return;
        case SDLK_n:
          elevation_idx = elevation_idx == (elevations.size()-1) ? 0 : elevation_idx + 1;
          break;
        case SDLK_p:
          elevation_idx = elevation_idx == 0 ? (elevations.size()-1) : elevation_idx-1;
          break;
        }
      }
    }
  }

  void show_map_strm(AppState& st, NamedStrm in) {
    try {
      st.window.set_title(in.name);
      map::File f = map::read_file(in.strm);
      render(st, f);
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

  CLI11_PARSE(app, argc, argv);

  try {
    sdl::Context c;
    sdl::Window w = c.create_window({ 1000, 1000 });
    AppState st{ read_pal(pal_pth), tiles_dir_pth, read_tiles(tiles_dir_pth), std::move(c), std::move(w), {}, {2000, 1000} };
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
