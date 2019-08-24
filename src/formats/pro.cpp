#include "src/formats/pro.hpp"

#include <stdexcept>
#include <sstream>

using klmth::pro::ObjectType;
using klmth::pro::FrmType;
using klmth::pro::WallOrientation;
using namespace klmth;

const char* pro::str(ObjectType type) {
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

const char* pro::str(FrmType type) {
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

bool pro::Flags::flat() const {
  return (val & 0x00000008) > 0;
}

bool pro::Flags::noblock() const {
  return (val & 0x00000010) > 0;
}

bool pro::Flags::multihex() const {
  return (val & 0x00000800) > 0;
}

bool pro::Flags::nohighlight() const {
  return (val & 0x00001000) > 0;
}

bool pro::Flags::transred() const {
  return (val & 0x00004000) > 0;
}

bool pro::Flags::transnone() const {
  return (val & 0x00008000) > 0;
}

bool pro::Flags::transwall() const {
  return (val & 0x00010000) > 0;
}

bool pro::Flags::transglass() const {
  return (val & 0x00020000) > 0;
}

bool pro::Flags::transsteam() const {
  return (val & 0x00040000) > 0;
}

bool pro::Flags::transenergy() const {
  return (val & 0x00080000) > 0;
}

bool pro::Flags::walltransend() const {
  return (val & 0x10000000) > 0;
}

bool pro::Flags::lightthru() const {
  return (val & 0x20000000) > 0;
}

bool pro::Flags::shootthru() const {
  return (val & 0x80000000) > 0;
}

std::vector<std::string> pro::flag_strs(Flags f) {
  std::vector<std::string> els;
  
  if (f.flat()) {
    els.push_back("flat");
  }
  if (f.noblock()) {
    els.push_back("noblock");
  }
  if (f.multihex()) {
    els.push_back("multihex");
  }
  if (f.nohighlight()) {
    els.push_back("nohighlight");
  }
  if (f.transred()) {
    els.push_back("transred");
  }
  if (f.transnone()) {
    els.push_back("transnone");
  }
  if (f.transwall()) {
    els.push_back("transwall");
  }
  if (f.transglass()) {
    els.push_back("transglass");
  }
  if (f.transsteam()) {
    els.push_back("transsteam");
  }
  if (f.transenergy()) {
    els.push_back("transenergy");
  }
  if (f.walltransend()) {
    els.push_back("walltransend");
  }
  if (f.lightthru()) {
    els.push_back("lightthru");
  }
  if (f.shootthru()) {
    els.push_back("shootthru");
  }

  return els;
}

const char* pro::str(WallOrientation orientation) {
  switch (orientation) {    
  case WallOrientation::north_south:
    return "north_south";
  case WallOrientation::east_west:
    return "east_west";
  case WallOrientation::north_corner:
    return "north_corner";
  case WallOrientation::south_corner:
    return "south_corner";
  case WallOrientation::east_corner:
    return "east_corner";
  case WallOrientation::west_corner:
    return "west_corner";
  default:
    throw std::runtime_error{"unknown orientation passed to pro::str(WallOrientation)"};
  }
}

bool pro::ActionFlags::kneel_when_using() const {
  return (val & 0x0001) > 0;
}

bool pro::ActionFlags::usable() const {
  return (val & 0x0008) > 0;
}

bool pro::ActionFlags::can_be_used_on_anything() const {
  return (val & 0x0010) > 0;
}

bool pro::ActionFlags::look() const {
  return (val & 0x0020) > 0;
}

bool pro::ActionFlags::talk() const {
  return (val & 0x0040) > 0;
}

bool pro::ActionFlags::pickup() const {
  return (val & 0x0080) > 0;
}

std::vector<std::string> pro::flag_strs(ActionFlags f) {
  std::vector<std::string> els;
  
  if (f.kneel_when_using()) {
    els.push_back("kneel_when_using");
  }
  if (f.usable()) {
    els.push_back("usable");
  }
  if (f.can_be_used_on_anything()) {
    els.push_back("can_be_used_on_anything");
  }
  if (f.look()) {
    els.push_back("look");
  }
  if (f.talk()) {
    els.push_back("talk");
  }
  if (f.pickup()) {
    els.push_back("pickup");
  }

  return els;
}

const char* pro::str(MaterialId id) {
  switch (id) {
  case MaterialId::glass:
    return "glass";
  case MaterialId::metal:
    return "metal";
  case MaterialId::plastic:
    return "plastic";
  case MaterialId::wood:
    return "wood";
  case MaterialId::dirt:
    return "dirt";
  case MaterialId::stone:
    return "stone";
  case MaterialId::cement:
    return "cement";
  case MaterialId::leather:
    return "leather";
  default:
    throw std::runtime_error{"unknown material id passed to pro::str(MaterialId)"};
  }
}

pro::ScriptId::ScriptId(uint32_t _val): val{_val} {
  uint8_t type = (val >> 24) & 0xff;
  if (type != 3) {
    std::stringstream msg;
    msg << "incorrect pro::ScriptId value (" << val << "). Can only have a type id of 3 (for holodisk script) but has a value of " << std::to_string(type);
    throw std::runtime_error{msg.str()};
  }
}

pro::ScriptType pro::ScriptId::type() const noexcept {
  // it's actually (val >> 24) & 0xff, but only one script type is
  // used in PRO files
  return ScriptType::holodisk;
}

uint16_t pro::ScriptId::id() const noexcept {
  return val & 0xffff;
}


const char* pro::str(ItemType item_type) {
  switch (item_type) {
  case ItemType::armor:
    return "armor";
  case ItemType::container:
    return "container";
  case ItemType::drug:
    return "drug";
  case ItemType::weapon:
    return "weapon";
  case ItemType::ammo:
    return "ammo";
  case ItemType::misc:
    return "misc";
  case ItemType::key:
    return "key";
  default:
    throw std::runtime_error{"unknown ItemType passed to pro::str"};
  }
}

const char* pro::str(SceneryType scenery_type) {
  switch (scenery_type) {
  case SceneryType::door:
    return "door";
  case SceneryType::stairs:
    return "stairs";
  case SceneryType::elevator:
    return "elevator";
  case SceneryType::ladder_bottom:
    return "ladder_bottom";
  case SceneryType::ladder_top:
    return "ladder_top";
  case SceneryType::generic:
    return "generic";
  default:
    throw std::runtime_error{"unknown SceneryType passed to pro::str"};
  }
}
