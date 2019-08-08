#include "src/formats/pro.hpp"

#include <stdexcept>
#include <sstream>

using klmth::pro::ObjectType;
using klmth::pro::FrmType;
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
