#include "src/frmshow.hpp"

#include <iostream>
#include <fstream>
#include <memory>

#include "klamath/pal.hpp"
#include "klamath/frm.hpp"
#include "src/sdl.hpp"


namespace {
  using namespace klmth;

  const std::string stdin_senteniel = "-";

  pal::File load_palette(const std::string& source) {
    std::ifstream pal_in;
    pal_in.open(source, std::ios::in | std::ios::binary);

    if (pal_in.bad()) {
      throw std::runtime_error(source + ": error when opening palette (.pal) file");
    }

    try {
      return pal::parse(pal_in);
    } catch (const std::exception& ex) {
      throw std::runtime_error(source + ": error when parsing palette (.pal) file: " + ex.what());
    }
  }

  sdl::Texture create_texture(sdl::Window& w, const pal::File& palette, const frm::Frame& frame) {
    size_t num_pixels = frame.width * frame.height;
    std::vector<klmth::Rgb> pixels;
    pixels.resize(num_pixels);
    for (size_t i = 0; i < num_pixels; ++i) {
      uint8_t idx = frame.color_indices[i];
      pixels[i] = palette.palette[idx] * 4;
    }

    return w.create_texture(pixels, frame.width, frame.height);
  }

  void show_frame(const pal::File& palette, const frm::Frame& frame) {
    sdl::Context c;
    sdl::Window w = c.create_window(frame.width, frame.height);
    
    sdl::Texture t = create_texture(w, palette, frame);

    w.render_clear();
    w.render_copy_fullscreen(t);
    w.render_present();

    SDL_Event e;    
    while (c.wait_for_event(&e)) {
      if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
	break;
      }
    }
  }

  void show_frm(const pal::File& palette, std::istream& frm_in, const std::string& in_name) {
    try {
      frm::read_header(frm_in);
      frm::read_frame(frm_in);
            frm::read_frame(frm_in);
	          frm::read_frame(frm_in);
		        frm::read_frame(frm_in);

			      frm::read_frame(frm_in);
			            frm::read_frame(frm_in);
      frm::Frame f = frm::read_frame(frm_in);
      show_frame(palette, f);
    } catch (const std::exception& ex) {
      throw std::runtime_error(in_name + ": error showing frm: " + ex.what());
    }
  }

  void show_frm(const pal::File& palette, const std::string& source) {
    if (source == stdin_senteniel) {
      show_frm(palette, std::cin, "stdin");
    } else {
      std::fstream frm_in;
      frm_in.open(source, std::ios::in | std::ios::binary);
      
      if (!frm_in.good()) {
	throw std::runtime_error(std::string(source) + ": error when opening frm file");
      }
      
      show_frm(palette, frm_in, source);
    }
  }

  void show_frms(const pal::File& palette, const std::vector<std::string>& sources) {
    for (auto&& source : sources) {
      show_frm(palette, source);
    }
  }
}


int klmth::frm_show_main(int argc, const char** argv) {
  if (argc == 0) {
    throw std::runtime_error("too few arguments provided to `frm_show_main`: this is a developer error");
  } else if (argc < 2) {
    const std::string usage = std::string("usage: ") + argv[0] + " <pal_file> [frm_file]...";
    
    std::cerr << argv[0] << ": too few arguments" << std::endl;
    std::cerr << usage << std::endl;
    return 1;
  }
  

  try {
    pal::File palette = load_palette(argv[1]);

    if (argc == 2) {
      show_frms(palette, { stdin_senteniel });
    } else {
      show_frms(palette, { argv + 2, argv + argc });
    }
    
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": " << ex.what() << std::endl;
    return 1;
  }
}
