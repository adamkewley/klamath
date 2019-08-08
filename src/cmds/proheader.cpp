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

  void print(ostream& out, const Header& h) {
    print_kv(out, "type", pro::str(h.obj_id.type));
    print_kv(out, "object_id", h.obj_id.val);
    print_kv(out, "text_id", h.text_id);
    print_kv(out, "frm_type", pro::str(h.frm_id.type));
    print_kv(out, "light_radius", h.light_radius);
    print_kv(out, "light_intensity", h.light_intensity);
    print_kv(out, "flags", join(", ", pro::flag_strs(h.flags)));
  }

  void run(ostream& out, NamedStrm& strm) {
    out << "[" << strm.name << "]" << std::endl;
    Header h = pro::parse_header(strm.strm);
    print(out, h);
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
          run(out, in);
        }
      }
    }
  }
}

int cmd_proheader(int argc, char** argv) {
  vector<string> pths;

  CLI::App app{"dump PRO file headers"};
  app.add_option("pro_file", pths, "path to pro file(s). '-' is interpreted as stdin.");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& ex) {
    return app.exit(ex);
  }

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
