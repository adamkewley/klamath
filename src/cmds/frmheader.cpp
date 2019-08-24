#include <iostream>
#include <fstream>

#include "third_party/CLI11.hpp"

#include "src/formats/frm_reader.hpp"
#include "src/utils/cli.hpp"


namespace {
  using namespace klmth;

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

  void print_stream(cli::NamedStream& in, std::ostream& out) {
    frm::Header h = frm::read_header(in.strm);

    out << "[" << in.name << "]" << std::endl;
    write_frmheader(h, out);
    out << std::endl;
  }
}

int cmd_frmheader(int argc, char** argv) {
  CLI::App app{"print FRM file header as plaintext"};
  std::vector<std::string> frm_paths;
  app.add_option("frm_file", frm_paths, "path to FRM file. '-' is interpreted as stdin. Supplying no paths will cause application to read FRM data from stdin");

  CLI11_PARSE(app, argc, argv);

  try {
    auto handler = [](cli::NamedStream& strm) {
                     print_stream(strm, std::cout);
                   };
    cli::handle_paths(frm_paths, handler);

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "frmheader: " << ex.what() << std::endl;
    return 1;
  }
}
