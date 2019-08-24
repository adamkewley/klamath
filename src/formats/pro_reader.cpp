#include "src/formats/pro_reader.hpp"

#include <stdexcept>
#include <sstream>

#include "src/utils/io.hpp"

using namespace klmth;
using std::runtime_error;
using std::istream;
using std::stringstream;
using klmth::read_be_u32;
using klmth::pro::Header;
using klmth::pro::ObjectId;
using klmth::pro::ObjectType;
using klmth::pro::FrmType;
using klmth::pro::FrmId;
using klmth::pro::WallOrientation;
using klmth::pro::WallData;
using klmth::pro::ActionFlags;
using klmth::pro::ScriptId;
using klmth::pro::MaterialId;
using klmth::pro::ItemData;
using klmth::pro::ItemType;
using klmth::pro::SceneryData;
using klmth::pro::SceneryType;

namespace {
  ObjectType read_obj_type(uint8_t c) {
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
    uint32_t b = read_be_u32(in);
    ObjectType type = read_obj_type(b >> 24);
    uint16_t val = b & 0xffff;

    return { type, val };
  }

  FrmType read_frm_type(uint8_t c) {
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
    uint32_t b = read_be_u32(in);
    FrmType type = read_frm_type(b >> 24);
    uint16_t val = b & 0xffff;

    return { type, val };
  }

  WallOrientation read_wall_orientation(istream& in) {
    uint16_t v = read_be_u16(in);
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

  nonstd::optional<ScriptId> read_script_id(istream& in) {
    int32_t v = read_be_i32(in);

    if (v == -1) {
      return nonstd::nullopt;
    } else {
      return {v};
    }
  }

  MaterialId read_material_id(istream& in) {
    uint32_t v = read_be_u32(in);

    switch (v) {
    case 0:
      return MaterialId::glass;
    case 1:
      return MaterialId::metal;
    case 2:
      return MaterialId::plastic;
    case 3:
      return MaterialId::wood;
    case 4:
      return MaterialId::dirt;
    case 5:
      return MaterialId::stone;
    case 6:
      return MaterialId::cement;
    case 7:
      return MaterialId::leather;
    default:
      stringstream ss;
      ss << "unknown material id (" << v << ") encountered in PRO file";
      throw std::runtime_error{ss.str()};
    }
  }

  ItemType read_item_type(istream& in) {
    uint32_t v = read_be_u32(in);
    switch (v) {
    case 0:
      return ItemType::armor;
    case 1:
      return ItemType::container;
    case 2:
      return ItemType::drug;
    case 3:
      return ItemType::weapon;
    case 4:
      return ItemType::ammo;
    case 5:
      return ItemType::misc;
    case 6:
      return ItemType::key;
    default:
      stringstream msg;
      msg << "unknown item type value (" << v << ") encountered";
      throw std::runtime_error{msg.str()};
    }
  }

  SceneryType read_scenery_type(istream& in) {
    uint32_t v = read_be_u32(in);
    switch (v) {
    case 0:
      return SceneryType::door;
    case 1:
      return SceneryType::stairs;
    case 2:
      return SceneryType::elevator;
    case 3:
      return SceneryType::ladder_bottom;
    case 4:
      return SceneryType::ladder_top;
    case 5:
      return SceneryType::generic;
    default:
      stringstream msg;
      msg << "unknown scenery type (" << v << ") encountered";
      throw std::runtime_error{msg.str()};
    }
  }
}

Header klmth::pro::read_header(std::istream& in) {
  Header h;
  h.obj_id = read_oid(in);
  h.text_id = read_be_u32(in);
  h.frm_id = read_frmid(in);
  h.light_radius = read_be_u32(in);
  h.light_intensity = read_be_u32(in);
  h.flags = { read_be_u32(in) };
  return h;
}

WallData pro::read_wall_data(std::istream& in) {
  WallData wd;
  wd.orientation = read_wall_orientation(in);
  wd.action_flags = { read_be_u16(in) };
  wd.script_id = read_script_id(in);
  wd.material_id = read_material_id(in);
  return wd;
}

ItemData pro::read_item_data(std::istream& in) {
  ItemData ret;

  in.ignore(3);  // flags ext: some parts for all items, some parts for weapons
  in.ignore(1);  // attack modes: only for weapons
  in.ignore(4);  // script ID

  ret.type = read_item_type(in);
  ret.material_id = read_material_id(in);
  ret.size = read_be_u32(in);
  ret.weight = read_be_u32(in);
  ret.cost = read_be_u32(in);
  ret.inventory_fid = read_be_u32(in);
  ret.sound_id = read_u8(in);

  return ret;
}

SceneryData pro::read_scenery_data(std::istream& in) {
  SceneryData ret;
  ret.orientation = read_wall_orientation(in);
  ret.action_flags = { read_be_u16(in) };
  ret.script_id = read_script_id(in);
  ret.type = read_scenery_type(in);
  ret.material_id = read_material_id(in);
  ret.sound_id = read_u8(in);
  return ret;
}
