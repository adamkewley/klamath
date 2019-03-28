#include "frmheader.hpp"

#include <iostream>
#include <fstream>

#include "klamath/frm.hpp"


namespace {
  using namespace klmth;

  const std::string stdin_senteniel = "-";


  void write_frmheader(const frm::Header& h, std::ostream& out) {
    out << "version number = " << h.version_number << std::endl
        << "fps = " << h.fps << std::endl
        << "action frame = " << h.action_frame << std::endl
        << "frames per direction = " << h.frames_per_direction << std::endl;

    out << "pixel shifts (x) = ";
    for (auto&& pixel_shift : h.pixel_shifts) {
      out << " " << std::to_string(pixel_shift.x);
    }
    out << std::endl;

    out << "pixel shifts (y) = ";
    for (auto&& pixel_shift : h.pixel_shifts) {
      out << " " << std::to_string(pixel_shift.y);
    }
    out << std::endl;

    out << "offsets in frame data = ";
    for (auto&& offset : h.offsets_in_frame_data) {
      out << " " << std::to_string(offset);
    }
    out << std::endl;
  }

  frm::Header read_header(std::istream& in, const std::string& in_name) {
    try {
      return frm::read_header(in);
    } catch (const std::exception& ex) {
      throw std::runtime_error(in_name + ": error parsing frm data: " + ex.what());
    }
  }

  void print_stream(std::istream& in, std::ostream& out, const std::string& in_name) {
    frm::Header h = read_header(in, in_name);

    out << "[" << in_name << "]" << std::endl;
    write_frmheader(h, out);
    out << std::endl;
  }

  void print_path(const std::string& path) {
    if (path == stdin_senteniel) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      std::fstream frm_in;
      frm_in.open(path, std::ios::in | std::ios::binary);

      if (!frm_in.good()) {
        throw std::runtime_error(std::string(path) + ": error when opening frm file");
      }

      print_stream(frm_in, std::cout, path);
    }
  }
}

int klmth::frmheader_main(int argc, const char** argv) {
  if (argc == 0) {
    throw std::runtime_error("too few arguments provided to `frm_show_main`: this is a developer error");
  }

  try {
    if (argc == 1) {
      print_stream(std::cin, std::cout, "stdin");
    } else {
      for (int i = 1; i < argc; ++i) {
        print_path(argv[i]);
      }
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": " << ex.what() << std::endl;
    return 1;
  }
}
