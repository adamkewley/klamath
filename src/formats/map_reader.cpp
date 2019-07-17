#include "src/formats/map_reader.hpp"

#include <istream>
#include <sstream>

#include "src/utils/io.hpp"

using namespace klmth;
using klmth::map::Header;
using klmth::map::File;
using klmth::map::Version;
using klmth::map::PlayerDefaults;
using klmth::map::Elevation;

namespace {
  Version read_version(std::istream& in) {
    auto v = read_be_u32_unsafe(in);
    switch (v) {
    case map::fallout_1:
    case map::fallout_2:
      return static_cast<Version>(v);
    default: {
      std::stringstream msg;
      msg << v << ": invalid version number in version field of MAP file";
      throw std::runtime_error{msg.str()};
    }
    }
  }

  std::string read_filename(std::istream& in) {
    char buf[17] = {0};
    in.read(buf, sizeof(buf)-1);

    if (in.gcount() != sizeof(buf)-1) {
      throw std::runtime_error{"ran out of data when trying to read MAP filename field"};
    }

    return {buf};
  }

  Elevation read_elevation(std::istream& in) {
    int32_t e = read_be_i32_unsafe(in);
    switch (e) {
    case 0:
      return map::low;
    case 1:
      return map::med;
    case 2:
      return map::high;
    default: {
      std::stringstream msg;
      msg << e << ": invalid elevation value in header";
      throw std::runtime_error{msg.str()};
    }
    }
  }

  frm::Orientation read_orientation(std::istream& in) {
    int o = read_be_i32_unsafe(in);
    switch (o) {
    case 0:
      return frm::north_east;
    case 1:
      return frm::east;
    case 2:
      return frm::south_east;
    case 3:
      return frm::south_west;
    case 4:
      return frm::west;
    case 5:
      return frm::north_west;
    default: {
      std::stringstream msg;
      msg << o << ": invalid orientation value in header";
      throw std::runtime_error{msg.str()};
    }
    }
  }

  PlayerDefaults read_player_defaults(std::istream& in) {
    PlayerDefaults ret;
    ret.pos = read_be_i32_unsafe(in);
    ret.elevation = read_elevation(in);
    ret.orientation = read_orientation(in);
    return ret;
  }
}

Header map::parse_header(std::istream& in) {
  Header ret;
  ret.version = read_version(in);
  ret.filename = read_filename(in);
  ret.player_defaults = read_player_defaults(in);
  ret.num_local_vars = read_be_i32_unsafe(in);
  ret.script_id = read_be_i32_unsafe(in);

  int32_t flg = read_be_i32_unsafe(in);

  ret.is_savegame_map = (flg & 0x1) != 0;
  ret.has_low_elevation = (flg & 0x2) == 0;
  ret.has_med_elevation = (flg & 0x4) == 0;
  ret.has_high_elevation = (flg & 0x8) == 0;
  
  read_be_i32_unsafe(in);  // map darkness (unused)

  ret.num_global_vars = read_be_i32_unsafe(in);
  ret.map_id = read_be_i32_unsafe(in);
  ret.timestamp = read_be_u32_unsafe(in);

  char throwaway_buf[4*44];
  in.read(throwaway_buf, sizeof(throwaway_buf));

  if (in.gcount() != 4*44) {
    throw std::runtime_error{"ran out of data when skipping to the end of a MAP header"};
  }    
    
  return ret;
}

File map::parse_file(std::istream& in) {
  Header h = parse_header(in);

  unsigned num_globals = static_cast<unsigned>(h.num_global_vars);
  std::vector<int32_t> globals;
  for (auto i = 0U; i < num_globals; ++i) {
    int32_t global_var = read_be_i32_unsafe(in);
    globals.push_back(global_var);
  }

  return { std::move(h), std::move(globals) };
}
