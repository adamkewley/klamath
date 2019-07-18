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

namespace {
  using namespace klmth;
  using std::this_thread::sleep_for;
  using std::chrono::milliseconds;
  using sdl::Context;
  using sdl::Window;
  using sdl::StaticTexture;

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

  struct Shared {
    pal::File pal;
    std::string tiles_dir;
    lst::File tiles;
  };

  struct NamedStrm {
    std::istream& strm;
    std::string name;
  };

  StaticTexture load_tile(const pal::File& p, Window& w, std::string& pth) {
    frm::File file = read_frm(pth);

    // assumption: tiles only *really* have one orientation.
    const frm::Animation& anim = file.animation(frm::north_east);

    if (anim.frames().empty()) {
      std::stringstream msg;
      msg << pth << ": tile contains no frames";
      throw std::runtime_error{msg.str()};
    }

    return w.texture(p, anim.frames().front());
  }

  using TileTextures = std::unordered_map<map::TileId, StaticTexture>;

  TileTextures load_floor_tiles(const Shared& st,
                                Window& w,
                                const map::Tiles& tiles) {
    TileTextures ret;
    const lst::File& id_map = st.tiles;
    
    for (map::Tile t : tiles) {
      uint16_t id = t.floor_id;

      if (id == map::null_tileid) {
        continue;
      }

      if (id >= id_map.size()) {
        std::stringstream msg;
        msg << id << ": tile ID in map file is higher than number of tiles in lookup (usually, TILES.LST), which contains " << id_map.size() << " entries";
        throw std::runtime_error{msg.str()};
      }
            
      if (ret.find(id) != ret.end()) {
        continue;
      }

      const std::string& tile_name = id_map[id];

      // todo: case-insensitive pathing
      std::string tile_path =
        file_exists(st.tiles_dir + upcase(tile_name)) ?
        st.tiles_dir + upcase(tile_name) :
        file_exists(st.tiles_dir + downcase(tile_name)) ?
        st.tiles_dir + downcase(tile_name) :
        st.tiles_dir + tile_name;

      ret.emplace(id, load_tile(st.pal, w, tile_path));
    }

    return ret;
  }

  void handle(const Shared& st, const map::Tiles& tiles) {
    Context c;
    Window w = c.create_window({ 1000, 1000 });
    TileTextures textures = load_floor_tiles(st, w, tiles);

    for (auto row = 0U; row < map::rows; ++row) {
      auto row_beg = map::cols * row;
      for (auto col = 0U; col < map::cols; ++col) {
        const map::Tile& tile = tiles.at(row_beg + col);

        if (tile.floor_id == map::null_tileid) {
          continue;
        }

        StaticTexture& texture = textures.at(tile.floor_id);
        sdl::Point pos{10*col, 10*row};
        sdl::Dimensions d{10, 10};
        w.render_texture(texture, {pos, d});
      }
    }
    w.render_present();
    sleep_for(milliseconds {1000});
  }

  void handle(const Shared& st, NamedStrm& in) {
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
      handle(st, *lowest);
    }    
  }

  void handle_map_strm(const Shared& st, NamedStrm in) {
    try {
      handle(st, in);
    } catch (const std::exception& ex) {
      std::stringstream msg;
      msg << in.name << ": " << ex.what();
      throw std::runtime_error{msg.str()};
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
    Shared st{ read_pal(pal_pth), tiles_dir_pth, read_tiles(tiles_dir_pth) };
    if (map_pths.empty()) {
      handle_map_strm(st, { std::cin, "stdin" });
    } else {
      for (const std::string& map_pth : map_pths) {
        if (map_pth == "-") {
          handle_map_strm(st, { std::cin, "stdin" });
        } else {
          std::ifstream in = open_file(map_pth);
          handle_map_strm(st, { in, map_pth });
        }
      }
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
