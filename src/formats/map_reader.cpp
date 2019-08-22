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
using klmth::map::Tiles;
using klmth::map::Elevation;
using klmth::map::Script;
using klmth::map::Pid;
using klmth::map::ScriptType;

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
      return Elevation::low;
    case 1:
      return Elevation::med;
    case 2:
      return Elevation::high;
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

  std::unique_ptr<Tiles> read_tiles(std::istream& in) {
    Tiles t;
    for (auto i = 0U; i < map::tiles_per_elevation; ++i) {
      uint16_t roof = read_be_u16_unsafe(in);
      uint16_t floor = read_be_u16_unsafe(in);
      t[i] = { roof, floor };
    }
    return std::make_unique<Tiles>(std::move(t));
  }

  Pid read_pid(std::istream& in) {
    return {read_be_u32_unsafe(in)};
  }

  std::vector<Script> read_scripts(std::istream& in) {
    std::vector<Script> ret;
    for (auto i = 0U; i < 5; ++i) {
      uint32_t count = read_be_u32_unsafe(in);

      if (count == 0) {
        continue;
      }

      uint32_t loop = count;
      if (count % 16 > 0) {
        loop += 16 - count % 16;
      }

      uint32_t check = 0;
      for (auto j = 0U; j < loop; ++j) {
        Pid pid = read_pid(in);
        read_be_u32_unsafe(in);  // next script. unused

        switch (pid.type()) {
        case ScriptType::spatial:
          read_be_u32_unsafe(in);  // spatial tile
          read_be_u32_unsafe(in);  // spatial radius
          break;
        case ScriptType::timer:
          read_be_u32_unsafe(in);  // time
          break;
        default:
          break;  // others have nothing to skip
        }

        read_be_u32_unsafe(in); //flags
        read_be_u32_unsafe(in); // script ID
        read_be_u32_unsafe(in); //unknown 5
        read_be_u32_unsafe(in); //oid == object->OID
        read_be_u32_unsafe(in); //local var offset
        read_be_u32_unsafe(in); //loal var cnt
        read_be_u32_unsafe(in); //unknown 9
        read_be_u32_unsafe(in); //unknown 10
        read_be_u32_unsafe(in); //unknown 11
        read_be_u32_unsafe(in); //unknown 12
        read_be_u32_unsafe(in); //unknown 13
        read_be_u32_unsafe(in); //unknown 14
        read_be_u32_unsafe(in); //unknown 15
        read_be_u32_unsafe(in); //unknown 16

        if (j < count) {
          ret.push_back({ pid });
        }

        if ((j % 16) == 15) {
          check += read_be_u32_unsafe(in);
          read_be_u32_unsafe(in);
        }
      }

      if (check != count) {
        throw std::runtime_error{"error reading scripts: check is incorrect"};
      }
    }
    
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
  const Header h = parse_header(in);

  std::vector<int32_t> globals = read_n_be_i32(in, h.num_global_vars);
  std::vector<int32_t> locals = read_n_be_i32(in, h.num_local_vars);

  std::unique_ptr<Tiles> low_level =
    h.has_low_elevation ? read_tiles(in) : nullptr;
  std::unique_ptr<Tiles> med_level =
    h.has_med_elevation ? read_tiles(in) : nullptr;
  std::unique_ptr<Tiles> high_level =
    h.has_high_elevation ? read_tiles(in) : nullptr;

  std::vector<Script> scripts = read_scripts(in);

  return { std::move(h), std::move(globals), std::move(locals), std::move(low_level), std::move(med_level), std::move(high_level), std::move(scripts) };
}
