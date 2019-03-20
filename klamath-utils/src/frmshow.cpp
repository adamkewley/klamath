#include "src/frmshow.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <SDL2/SDL_events.h>

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

  sdl::Texture create_texture(sdl::Window& w, const pal::File& palette, const frm::Image& img) {
    auto& color_indices = img.color_indices();
    size_t num_pixels = color_indices.size();
    std::vector<klmth::Rgb> pixels;
    pixels.resize(num_pixels);
    for (size_t i = 0; i < num_pixels; ++i) {
      uint8_t idx = color_indices[i];
      pixels[i] = palette.palette[idx] * 4;
    }

    return w.create_texture(pixels, { img.width(), img.height() });
  }

  void show_image(const pal::File& palette, const frm::Image& img) {
    sdl::Context c;
    sdl::Window w = c.create_window({ img.width(), img.height() });
    
    sdl::Texture t = create_texture(w, palette, img);

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

  void show_animation(const pal::File& palette, const frm::Animation& animation) {
    sdl::Context c;
    sdl::Window w = c.create_window({ animation.width(), animation.height() });

    std::vector<sdl::Texture> frame_textures;
    frame_textures.reserve(animation.num_frames());
    for (const frm::Image& frame : animation.frames()) {
      frame_textures.emplace_back(create_texture(w, palette, frame));
    }

    const std::chrono::milliseconds sleep_dur(1000/animation.fps());
    const size_t num_frames = animation.num_frames();

    SDL_Event e;
    int frame = 0;

    while (true) {
      while (c.poll_event(&e)) {
	if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
	  return;
	}
      }
      w.render_clear();
      w.render_copy_fullscreen(frame_textures[frame++ % num_frames]);
      w.render_present();
      sdl::sleep(sleep_dur);
    }
  }

  class OrientationsLayout {
  public:
    OrientationsLayout(const frm::Orientable& orientable) noexcept : _cell_dimensions(orientable.dimensions()) {
    }

    unsigned rows() const noexcept {
      return 3;
    }

    unsigned cols() const noexcept {
      return 2;
    }

    sdl::Dimensions dimensions() const noexcept {
      auto width = this->_cell_dimensions.width * cols();
      auto height = this->_cell_dimensions.height * rows();
      return { width, height };
    }
    
    sdl::Rect rect(frm::Orientation orientation) const noexcept {
      return { cell_loc(orientation), { _cell_dimensions.width, _cell_dimensions.height } };
    }

    sdl::Point cell_loc(frm::Orientation orientation) const {
      switch (orientation) {
      case frm::Orientation::north_east:
	return { _cell_dimensions.width, 0 };
      case frm::Orientation::east:
	return { _cell_dimensions.width, _cell_dimensions.height };
      case frm::Orientation::south_east:
	return { _cell_dimensions.width, _cell_dimensions.height * 2u };
      case frm::Orientation::south_west:
	return { 0, _cell_dimensions.height * 2u };
      case frm::Orientation::west:
	return { 0, _cell_dimensions.height };
      case frm::Orientation::north_west:
	return { 0, 0 };
      default:
	throw std::runtime_error("unknown orientation: cannot calculate location for this orientation in the layout");
      }
    }
    
  private:
    frm::Dimensions _cell_dimensions;
  };

  void show_orientable(const pal::File& palette, const frm::Orientable& orientable) {
    OrientationsLayout layout{orientable};
    
    sdl::Context c;
    sdl::Window w = c.create_window(layout.dimensions());

    w.render_clear();

    for (frm::Orientation orientation : frm::orientations) {
      sdl::Texture t = create_texture(w, palette, orientable.image_at(orientation));
      w.render_copy(t, layout.rect(orientation));
    }

    w.render_present();
    
    SDL_Event e;    
    while (c.wait_for_event(&e)) {
      if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
	break;
      }
    }
  }

  void show_animated_orientable(const pal::File& palette, const frm::AnimatedOrientable& anim_orientable) {
    throw std::runtime_error("showing animated orientables NYI");
  }

  void show_frm(const pal::File& palette, std::istream& frm_in, const std::string& in_name) {
    try {
      frm::Any any = frm::read_any(frm_in);
      switch (any.type()) {
      case frm::AnyType::image:
	show_image(palette, any.image_unpack());
	break;
      case frm::AnyType::animation:
	show_animation(palette, any.animation_unpack());
	break;
      case frm::AnyType::orientable:
	show_orientable(palette, any.orientable_unpack());
	break;
      case frm::AnyType::animated_orientable:
	show_animated_orientable(palette, any.animated_orientable_unpack());
	break;
      }
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
