#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"
#include "src/formats/lst_reader.hpp"
#include "src/formats/frm_reader.hpp"
#include "src/formats/pal_reader.hpp"

namespace {
  using namespace klmth;

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

  struct Shared {
    pal::File pal;
    lst::File tiles;
  };

  struct NamedStrm {
    std::istream& strm;
    std::string name;
  };

  void handle_map_strm(const Shared& st, NamedStrm in) {
    map::File f = map::parse_file(in.strm);
    
    // todo
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
    Shared st{ read_pal(pal_pth), read_tiles(tiles_dir_pth) };
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
