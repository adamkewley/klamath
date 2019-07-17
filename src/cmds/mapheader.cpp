#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/map.hpp"
#include "src/formats/map_reader.hpp"

namespace {
  using namespace klmth;
  using klmth::map::Header;
  using klmth::map::Version;
  using klmth::map::PlayerDefaults;
  using klmth::map::Elevation;
  using klmth::map::parse_header;

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
    case map::low:
      return "low";
    case map::med:
      return "med";
    case map::high:
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
  
  void print_stream(std::istream& in, std::ostream& out, const std::string& stream_name) {
    print_header(parse_header(in), out, stream_name);
  }

  void print_stdin(std::ostream& out) {
    print_stream(std::cin, out, "stdin");
  }

  void print_path(const std::string& path, std::ostream& out) {
    if (path == "-") {
      print_stdin(out);
      return;
    }

    std::fstream in{path, std::ios::in | std::ios::binary};
    if (!in.good()) {
      std::stringstream err{};
      err << path << ": " << strerror(errno);
      throw std::runtime_error{err.str()};
    }

    print_stream(in, out, path);
  }

  void print_paths(const std::vector<std::string>& paths, std::ostream& out) {
    for (const std::string& path : paths) {
      print_path(path, out);
    }
  }
}

int cmd_mapheader(int argc, char** argv) {
  CLI::App app{"print MAP file header as plaintext"};
  std::vector<std::string> paths;
  app.add_option("map_file", paths, "path to MAP file. '-' is interpreted as stdin. Supplying no paths will cause application to read MAP data from stdin");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& ex) {
    return app.exit(ex);
  }

  try {
    if (paths.empty()) {
      print_stdin(std::cout);
    } else {
      print_paths(paths, std::cout);      
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "mapheader: " << ex.what() << std::endl;
    return 1;
  }
}
