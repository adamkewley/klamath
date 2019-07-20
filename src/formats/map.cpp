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
