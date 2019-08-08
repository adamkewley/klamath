#include "src/formats/pro_reader.hpp"

#include <stdexcept>
#include <sstream>

#include "src/utils/io.hpp"

using namespace klmth;
using std::runtime_error;
using std::istream;
using std::stringstream;
using klmth::read_be_u32_unsafe;
using klmth::pro::Header;
using klmth::pro::ObjectId;
using klmth::pro::ObjectType;
using klmth::pro::FrmType;
using klmth::pro::FrmId;
using klmth::pro::WallOrientation;
using klmth::pro::WallData;
using klmth::pro::ActionFlags;

namespace {
  ObjectType parse_obj_type(uint8_t c) {
    switch (c) {
    case 0:
      return ObjectType::item;
    case 1:
      return ObjectType::critter;
    case 2:
      return ObjectType::scenery;
    case 3:
      return ObjectType::wall;
    case 4:
      return ObjectType::tile;
    case 5:
      return ObjectType::misc;
    default:
      stringstream ss;
      ss << "unknown object type (" << c << ") encountered in PRO file";
      throw runtime_error{ss.str()};
    }
  }
  
  ObjectId read_oid(istream& in) {
    uint32_t b = read_be_u32_unsafe(in);
    ObjectType type = parse_obj_type(b >> 24);
    uint16_t val = b & 0xffff;

    return { type, val };
  }

  FrmType parse_frm_type(uint8_t c) {
    switch (c) {
    case 0:
      return FrmType::item;
    case 1:
      return FrmType::critter;
    case 2:
      return FrmType::scenery;
    case 3:
      return FrmType::wall;
    case 4:
      return FrmType::tile;
    case 5:
      return FrmType::background;
    case 6:
      return FrmType::interface;
    case 7:
      return FrmType::inventory;
    default:
      stringstream ss;
      ss << "unknown frm type (" << c << ") encountered in PRO file";
      throw runtime_error{ss.str()};
    }
  }

  FrmId read_frmid(istream& in) {
    uint32_t b = read_be_u32_unsafe(in);
    FrmType type = parse_frm_type(b >> 24);
    uint16_t val = b & 0xffff;
    
    return { type, val };
  }

  WallOrientation read_wall_orientation(istream& in) {
    uint16_t v = read_be_u16_unsafe(in);
    switch (v) {
    case 0x0000:
      return WallOrientation::north_south;
    case 0x0800:
      return WallOrientation::east_west;
    case 0x1000:
      return WallOrientation::north_corner;
    case 0x2000:
      return WallOrientation::south_corner;
    case 0x4000:
      return WallOrientation::east_corner;
    case 0x8000:
      return WallOrientation::west_corner;
    default:
      stringstream ss;
      ss << "unknown wall orientation value (" << v << "encountered in PRO file";
      throw std::runtime_error{ss.str()};
    }
  }
}

Header klmth::pro::parse_header(std::istream& in) {
  Header h;
  h.obj_id = read_oid(in);
  h.text_id = read_be_u32_unsafe(in);
  h.frm_id = read_frmid(in);
  h.light_radius = read_be_u32_unsafe(in);
  h.light_intensity = read_be_u32_unsafe(in);
  h.flags = { read_be_u32_unsafe(in) };
  return h;
}

WallData pro::parse_wall_data(std::istream& in) {
  WallData wd;
  wd.orientation = read_wall_orientation(in);
  wd.action_flags = { read_be_u16_unsafe(in) };
  return wd;
}
