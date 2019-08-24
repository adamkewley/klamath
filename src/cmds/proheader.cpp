#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/pro.hpp"
#include "src/formats/pro_reader.hpp"
#include "src/utils/cli.hpp"

using klmth::pro::Header;
using klmth::pro::ObjectType;
using klmth::pro::WallData;
using klmth::pro::ItemData;
using klmth::pro::SceneryData;
using namespace klmth;

namespace {
  std::string join(const std::string& delim, const std::vector<std::string>& els) {
    if (els.size() == 0) {
      return "";
    }

    std::ostringstream ss;
    auto it = els.begin();
    auto end = els.end() - 1;
    for (; it < end; ++it) {
      ss << *it << delim;
    }
    ss << *end;

    return ss.str();
  }

  template<typename T>
  void print_kv(std::ostream& out, const char* k, T v) {
    out << k << " = " << v << std::endl;
  }

  void read_and_print_wall(std::ostream& out, std::istream& in) {
    WallData wd = pro::read_wall_data(in);
    print_kv(out, "wall_orientation", pro::str(wd.orientation));
    print_kv(out, "action_flags", join(", ", pro::flag_strs(wd.action_flags)));
    print_kv(out, "material_id", pro::str(wd.material_id));
  }

  void read_and_print_item(std::ostream& out, std::istream& in) {
    ItemData item = pro::read_item_data(in);
    print_kv(out, "item_type", pro::str(item.type));
    print_kv(out, "material_id", pro::str(item.material_id));
    print_kv(out, "size", item.size);
    print_kv(out, "weight", item.weight);
    print_kv(out, "cost", item.weight);
    print_kv(out, "inventory_fid", item.inventory_fid);
    print_kv(out, "sound_id", item.sound_id);
  }

  void read_and_print_scenery(std::ostream& out, std::istream& in) {
    SceneryData scenery = pro::read_scenery_data(in);
    print_kv(out, "orientation", pro::str(scenery.type));
    print_kv(out, "action_flags", join(", ", pro::flag_strs(scenery.action_flags)));
  }

  void print(std::ostream& out, const Header& h, std::istream& in) {
    print_kv(out, "type", pro::str(h.obj_id.type));
    print_kv(out, "object_id", h.obj_id.val);
    print_kv(out, "text_id", h.text_id);
    print_kv(out, "frm_type", pro::str(h.frm_id.type));
    print_kv(out, "light_radius", h.light_radius);
    print_kv(out, "light_intensity", h.light_intensity);
    print_kv(out, "flags", join(", ", pro::flag_strs(h.flags)));

    switch (h.obj_id.type) {
    case ObjectType::wall:
      read_and_print_wall(out, in);
      break;
    case ObjectType::item:
      read_and_print_item(out, in);
      break;
    case ObjectType::scenery:
      read_and_print_scenery(out, in);
      break;
    default:
      break;  // todo: other pro data types
    }
  }

  void run(std::ostream& out, cli::NamedStream& strm) {
    out << "[" << strm.name << "]" << std::endl;
    Header h = pro::read_header(strm.strm);
    print(out, h, strm.strm);
    out << std::endl;
  }
}

int cmd_proheader(int argc, char** argv) {
  CLI::App app{"dump PRO file headers"};
  std::vector<std::string> paths;
  app.add_option("pro_file", paths, "path to pro file(s). '-' is interpreted as stdin.");

  CLI11_PARSE(app, argc, argv);

  try {
    auto handler = [](cli::NamedStream& strm) {
                     run(std::cout, strm);
                   };

    cli::handle_paths(paths, handler);

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "proheader: " << ex.what() << std::endl;
    return 1;
  }
}
