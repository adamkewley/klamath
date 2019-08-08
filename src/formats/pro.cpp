#include "src/formats/pro.hpp"

#include <stdexcept>

using klmth::pro::ObjectType;

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
