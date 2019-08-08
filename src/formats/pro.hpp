#pragma once

#include <cstdint>

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
      uint32_t val;

      bool is_flat() const;
      bool is_noblock() const;
      bool is_multihex() const;
      bool is_nohighlight() const;
      bool is_transred() const;
      bool is_transnone() const;
      bool is_transwall() const;
      bool is_transglass() const;
      bool is_transsteam() const;
      bool is_transenergy() const;
      bool is_walltransend() const;
      bool is_lightthru() const;
      bool is_shootthru() const;
    };

    struct Header {
      ObjectId obj_id;
      uint32_t text_id;  // object name and description in text/english/game/pro_item.msg, etc.
      FrmId frm_id;
      uint32_t light_radius;
      uint32_t light_intensity;
      Flags flags;
    };

    struct WallLightTypeFlags {
      uint16_t val;

      bool is_north_or_south() const;
      bool is_east_or_west() const;
      bool is_north_corner() const;
      bool is_south_corner() const;
      bool is_east_corner() const;
      bool is_west_corner() const;
    };

    struct ActionFlags {
      uint16_t val;
      
      bool kneel_when_using() const;
      bool is_usable() const;
      bool is_usable_on_anything() const;
      bool can_look() const;
      bool can_talk() const;
      bool can_pick_up() const;
    };

    enum class ScriptType {
      holodisk,
    };

    struct ScriptId {
      ScriptType type;
      uint16_t val;
    };

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

    struct Wall {
      Header header;
      WallLightTypeFlags wall_light_flags;
      ActionFlags action_flags;
      ScriptId script_id;
      MaterialId material;
    };
  }
}
