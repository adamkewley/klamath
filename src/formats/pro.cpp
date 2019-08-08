#include "src/formats/pro.hpp"

#include <stdexcept>

using klmth::pro::ObjectType;
using klmth::pro::FrmType;

const char* klmth::pro::str(ObjectType type) {
  switch (type) {
  case ObjectType::item:
    return "item";
  case ObjectType::critter:
    return "critter";
  case ObjectType::scenery:
    return "scenery";
  case ObjectType::wall:
    return "wall";
  case ObjectType::tile:
    return "tile";
  case ObjectType::misc:
    return "misc";
  default:
    throw std::runtime_error{"unknown object type passed into pro::str(ObjectType)"};
  }
}

const char* klmth::pro::str(FrmType type) {
  switch (type) {
  case FrmType::item:
    return "item";
  case FrmType::critter:
    return "critter";
  case FrmType::scenery:
    return "scenery";
  case FrmType::wall:
    return "wall";
  case FrmType::tile:
    return "tile";
  case FrmType::background:
    return "background";
  case FrmType::interface:
    return "interface";
  case FrmType::inventory:
    return "inventory";
  default:
    throw std::runtime_error{"unknown object passed into pro::str(FrmType)"};
  }
}
