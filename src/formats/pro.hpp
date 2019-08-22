#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "third_party/optional.hpp"

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
      
      uint32_t val;
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

    struct ActionFlags {
      bool kneel_when_using() const;
      bool usable() const;
      bool can_be_used_on_anything() const;
      bool look() const;
      bool talk() const;
      bool pickup() const;

      uint16_t val;
    };

    std::vector<std::string> flag_strs(ActionFlags action_flags);

    enum class MaterialId {
      glass,
      metal,
      plastic,
      wood,
      dirt,
      stone,
      cement,
      leather,
    };

    const char* str(MaterialId material_id);

    enum class ScriptType {
      holodisk,
    };

    class ScriptId {
    public:
      ScriptId(uint32_t _val);
      ScriptType type() const noexcept;
      uint16_t id() const noexcept;
    private:
      uint32_t val;
    };

    struct WallData {
      WallOrientation orientation;
      ActionFlags action_flags;
      nonstd::optional<ScriptId> script_id;
      MaterialId material_id;
    };
  }
}
