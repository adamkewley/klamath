#include "src/formats/map.hpp"

using namespace klmth;

const std::unique_ptr<map::Tiles>& map::File::tiles(map::Elevation e) const {
    switch (e) {
    case map::Elevation::low:
        return this->low_elevation_tiles;
    case map::Elevation::med:
        return this->mid_elevation_tiles;
    case map::Elevation::high:
        return this->high_elevation_tiles;
    default:
        throw std::runtime_error{"Unknown elevation encountered"};
    }
}

std::vector<map::Elevation> map::File::elevations() const noexcept {
    std::vector<map::Elevation> ret;
    if (this->low_elevation_tiles) {
        ret.push_back(map::Elevation::low);
    }
    if (this->mid_elevation_tiles) {
        ret.push_back(map::Elevation::med);
    }
    if (this->high_elevation_tiles) {
        ret.push_back(map::Elevation::high);
    }
    return ret;
}

map::Pid::Pid(uint32_t val) {
  switch ((val & 0xff000000) >> 24) {
  case 0:
    this->_type = ScriptType::system;
    break;
  case 1:
    this->_type = ScriptType::spatial;
    break;
  case 2:
    this->_type = ScriptType::timer;
    break;
  case 3:
    this->_type = ScriptType::item;
    break;
  case 4:
    this->_type = ScriptType::critter;
    break;
  default:
    throw std::runtime_error{"Unknown script type in map PID"};
  }
}

map::ScriptType map::Pid::type() const noexcept {
  return this->_type;
}

char const * map::str(ScriptType type) {
  switch (type) {
  case ScriptType::system:
    return "system";
  case ScriptType::spatial:
    return "spatial";
  case ScriptType::timer:
    return "timer";
  case ScriptType::item:
    return "item";
  case ScriptType::critter:
    return "critter";
  default:
    throw std::runtime_error{"Unknown script type encountered"};
  }
}
