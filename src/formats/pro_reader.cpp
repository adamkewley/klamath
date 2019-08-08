#include "src/formats/pro_reader.hpp"

#include <stdexcept>
#include <sstream>

#include "src/utils/io.hpp"

using std::runtime_error;
using std::istream;
using std::stringstream;
using klmth::read_be_u32_unsafe;
using klmth::pro::Header;
using klmth::pro::ObjectId;
using klmth::pro::ObjectType;
using klmth::pro::FrmType;
using klmth::pro::FrmId;

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
}

Header klmth::pro::parse_header(std::istream& in) {
  Header h;
  h.obj_id = read_oid(in);
  h.text_id = read_be_u32_unsafe(in);
  h.frm_id = read_frmid(in);
  return h;
}
