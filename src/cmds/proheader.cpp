#include <vector>
#include <string>
#include <cstring>

#include "third_party/CLI11.hpp"

#include "src/formats/pro.hpp"
#include "src/formats/pro_reader.hpp"

using std::vector;
using std::string;
using std::istream;
using std::ifstream;
using std::stringstream;
using std::ostream;
using klmth::pro::Header;
using klmth::pro::ObjectType;
using klmth::pro::WallData;
using klmth::pro::ItemData;
using klmth::pro::SceneryData;

namespace {
  using namespace klmth;

  struct NamedStrm {
    istream& strm;
    string name;
  };

  ifstream open_file(const string& pth) {
    ifstream f{pth, std::ios::in | std::ios::binary};

    if (f.good()) {
      f.exceptions(std::ifstream::badbit);
      return f;
    } else {
      stringstream err;
      err << pth << ": " << strerror(errno);
      throw std::runtime_error{err.str()};
    }
  }

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
  void print_kv(ostream& out, const char* k, T v) {
    out << k << " = " << v << std::endl;
  }

  void read_and_print_wall(ostream& out, istream& in) {
    WallData wd = pro::read_wall_data(in);
    print_kv(out, "wall_orientation", pro::str(wd.orientation));
    print_kv(out, "action_flags", join(", ", pro::flag_strs(wd.action_flags)));
    print_kv(out, "material_id", pro::str(wd.material_id));
  }

  void read_and_print_item(ostream& out, istream& in) {
    ItemData item = pro::read_item_data(in);
    print_kv(out, "item_type", pro::str(item.type));
    print_kv(out, "material_id", pro::str(item.material_id));
    print_kv(out, "size", item.size);
    print_kv(out, "weight", item.weight);
    print_kv(out, "cost", item.weight);
    print_kv(out, "inventory_fid", item.inventory_fid);
    print_kv(out, "sound_id", item.sound_id);
  }

  void read_and_print_scenery(ostream& out, istream& in) {
    SceneryData scenery = pro::read_scenery_data(in);
    print_kv(out, "orientation", pro::str(scenery.type));
    print_kv(out, "action_flags", join(", ", pro::flag_strs(scenery.action_flags)));
  }

  void print(ostream& out, const Header& h, istream& in) {
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

  void run(ostream& out, NamedStrm& strm) {
    out << "[" << strm.name << "]" << std::endl;
    Header h = pro::read_header(strm.strm);
    print(out, h, strm.strm);
    out << std::endl;
  }

  void run(ostream& out, const vector<string>& pths) {
    if (pths.empty()) {
      NamedStrm stdin{ std::cin, "stdin" };
      run(out, stdin);
    } else {
      for (const string& pth : pths) {
        if (pth == "-") {
          NamedStrm stdin{ std::cin, "stdin" };
          run(out, stdin);
        } else {
          ifstream fd = open_file(pth);
          NamedStrm in{ fd, pth };
          try {
            run(out, in);
          } catch (const std::exception& ex) {
            std::stringstream msg;
            msg << pth << ": " << ex.what();
            throw std::runtime_error{msg.str()};
          }
        }
      }
    }
  }
}

int cmd_proheader(int argc, char** argv) {
  vector<string> pths;

  CLI::App app{"dump PRO file headers"};
  app.add_option("pro_file", pths, "path to pro file(s). '-' is interpreted as stdin.");

  CLI11_PARSE(app, argc, argv);

  try {
    run(std::cout, pths);
    return 0;
  } catch (const std::ios_base::failure& ex) {
    std::cerr << "proheader: " << ex.what() << ": " << strerror(errno) << std::endl;
    return 1;
  } catch (const std::exception& ex) {
    std::cerr << "proheader: " << ex.what() << std::endl;
    return 1;
  }
}
