#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"
#include "src/utils/cli.hpp"

using namespace klmth;

namespace {
  using klmth::map::Header;
  using klmth::map::Version;
  using klmth::map::PlayerDefaults;
  using klmth::map::Elevation;
  using klmth::map::read_header;
  using klmth::map::Elevation;

  const char* version_str(Version v) {
    switch (v) {
    case map::fallout_1:
      return "fallout_1";
    case map::fallout_2:
      return "fallout_2";
    default:
      throw std::runtime_error{"unknown version: cannot print"};
    }
  }

  const char* elevation_str(Elevation e) {
    switch (e) {
    case Elevation::low:
      return "low";
    case Elevation::med:
      return "med";
    case Elevation::high:
      return "high";
    default:
      throw std::runtime_error{"unknown elevation: cannot print"};
    }
  }

  void print_section(std::ostream& out, const std::string& section_name) {
    out << "[" << section_name << "]" << std::endl;
  }

  template<typename T>
  void print_kv(std::ostream& out, const char* k, T v) {
    out << k << " = " << v << std::endl;
  }

  void print_header(const Header& header, std::ostream& out, const std::string& stream_name) {
    print_section(out, stream_name);
    print_kv(out, "version", version_str(header.version));
    print_kv(out, "filename", header.filename);
    print_kv(out, "default_player_position", header.player_defaults.pos);
    print_kv(out, "default_player_elevation", elevation_str(header.player_defaults.elevation));
    print_kv(out, "default_player_orientation", header.player_defaults.orientation);
    print_kv(out, "num_local_vars", header.num_local_vars);
    print_kv(out, "script_id", header.script_id);
    print_kv(out, "is_savegame_map", header.is_savegame_map);
    print_kv(out, "has_low_elevation", header.has_low_elevation);
    print_kv(out, "has_med_elevation", header.has_med_elevation);
    print_kv(out, "has_high_elevation", header.has_high_elevation);
    print_kv(out, "num_global_vars", header.num_global_vars);
    print_kv(out, "map_id", header.map_id);
    print_kv(out, "timestamp", header.timestamp);
    out << std::endl;
  }
  
  void print_stream(cli::NamedStream& in, std::ostream& out) {
    print_header(read_header(in.strm), out, in.name);
  }
}

int cmd_mapheader(int argc, char** argv) {
  CLI::App app{"print MAP file header as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file. '-' is interpreted as stdin. Supplying no paths will cause application to read MAP data from stdin");

  CLI11_PARSE(app, argc, argv);

  auto path_handler = [](cli::NamedStream& strm) {
                        print_stream(strm, std::cout);
                      };

  return cli::main_with_paths("mapheader", paths, path_handler);
}
