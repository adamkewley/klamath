#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"
#include "src/utils/cli.hpp"

using namespace klmth;

namespace {
  using klmth::map::File;
  using klmth::map::Tiles;

  void print_tiles(const Tiles& tiles, std::ostream& out) {
    out << "[floor]" << std::endl;
    for (auto row = 0U; row < map::rows; ++row) {
      auto start = map::cols * row;
      for (auto col = 0U; col < map::cols; ++col) {
        uint16_t floor = tiles[start + col].floor_id;
        if (floor == 1) {
          out << "     ";
        } else {
          out << std::setfill(' ') << std::setw(5) << floor;
        }
      }
      out << std::endl;
    }
    
    out << "[roof]" << std::endl;
    for (auto row = 0U; row < map::rows; ++row) {
      auto start = map::cols * row;
      for (auto col = 0U; col < map::cols; ++col) {
        uint16_t roof = tiles[start + col].roof_id;
        if (roof == 1) {
          out << "     ";
        } else {
          out << std::setfill(' ') << std::setw(5) << roof;
        }
      }
      out << std::endl;
    }
  }

  void handle_output(cli::NamedStream in, std::ostream& out) {
    out << "[[[" << in.name << "]]]" << std::endl;
    File f = map::read_file(in.strm);
    
    if (f.low_elevation_tiles) {
      out << "[[low]]" << std::endl;
      print_tiles(*f.low_elevation_tiles, out);
    }

    if (f.mid_elevation_tiles) {
      out << "[[mid]]" << std::endl;
      print_tiles(*f.mid_elevation_tiles, out);
    }

    if (f.high_elevation_tiles) {
      out << "[[high]]" << std::endl;
      print_tiles(*f.high_elevation_tiles, out);      
    }
  }
}

int cmd_maptiles(int argc, char** argv) {
  
  CLI::App app{"dump MAP file tiles as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file. '-' is interpreted as stdin. Supplying no paths will cause application to read MAP from stdin. WARNING: prints out 'a lot' of ASCII: you should zoom your terminal out.");

  CLI11_PARSE(app, argc, argv);

  try {
    auto handler = [](cli::NamedStream& strm) {
                     handle_output(strm, std::cout);
                   };

    cli::handle_paths(paths, handler);
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "maptiles: " << ex.what() << std::endl;
    return 1;
  }
}
