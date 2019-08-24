#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"

namespace {
  using namespace klmth;
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

  void handle_output(std::istream& in, std::ostream& out, const std::string& stream_name) {
    out << "[[[" << stream_name << "]]]" << std::endl;
    File f = map::read_file(in);
    
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
  
  void print_stream(std::istream& in, std::ostream& out, const std::string& stream_name) {
    try {
      handle_output(in, out, stream_name);
    } catch (const std::exception& ex) {
      throw std::runtime_error(stream_name + ": " + ex.what());
    }
  }
  
  void print_path(const std::string& path, std::ostream& out) {
    std::fstream in{path, std::ios::in | std::ios::binary};
    
    if (!in.good()) {
      std::stringstream err{};
      err << path << ": " << strerror(errno);
      throw std::runtime_error{err.str()};
    }

    print_stream(in, out, path);
  }
}

int cmd_maptiles(int argc, char** argv) {
  
  CLI::App app{"dump MAP file tiles as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file. '-' is interpreted as stdin. Supplying no paths will cause application to read MAP from stdin. WARNING: prints out 'a lot' of ASCII: you should zoom your terminal out.");

  CLI11_PARSE(app, argc, argv);

  try {
    if (paths.empty()) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      for (const auto& path : paths) {
        if (path == "-") {
          print_stream(std::cin, std::cout, "stdin");
        } else {
          print_path(path, std::cout);
        }
      }
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "maptiles: " << ex.what() << std::endl;
    return 1;
  }
}
