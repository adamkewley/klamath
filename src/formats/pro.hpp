#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace klmth {
  namespace pro {
    
    enum class ObjectType {
      item,
      critter,
      scenery,
      wall,
      tile,
      misc,
    };

    const char* str(ObjectType type);
    
    struct ObjectId {
      ObjectType type;
      uint16_t val;
    };

    enum class FrmType {
      item,
      critter,
      scenery,
      wall,
      tile,
      background,
      interface,
      inventory,
    };

    const char* str(FrmType type);

    struct FrmId {
      FrmType type;
      uint16_t val;
    };

    struct Flags {
    public:      
      bool flat() const;
      bool noblock() const;
      bool multihex() const;
      bool nohighlight() const;
      bool transred() const;
      bool transnone() const;
      bool transwall() const;
      bool transglass() const;
      bool transsteam() const;
      bool transenergy() const;
      bool walltransend() const;
      bool lightthru() const;
      bool shootthru() const;
      
      uint32_t val = 0;
    };

    std::vector<std::string> flag_strs(Flags flags);

    struct Header {
      ObjectId obj_id;
      // object name and description in
      // text/english/game/pro_item.msg, etc.
      uint32_t text_id; 
      FrmId frm_id;
      uint32_t light_radius;
      uint32_t light_intensity;
      Flags flags;
    };

    enum class WallOrientation {
      north_south,
      east_west,
      north_corner,
      south_corner,
      east_corner,
      west_corner,
    };

    const char* str(WallOrientation orientation);

    struct WallData {
      WallOrientation orientation;
    };
  }
}
