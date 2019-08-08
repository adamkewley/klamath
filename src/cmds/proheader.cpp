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

  void print(ostream& out, const Header& h) {
    out << "type = " << pro::str(h.obj_id.type) << std::endl;
    out << "object_id = " << h.obj_id.val << std::endl;
    out << "text_id = " << h.text_id << std::endl;
    out << "frm_type = " << pro::str(h.frm_id.type) << std::endl;
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
