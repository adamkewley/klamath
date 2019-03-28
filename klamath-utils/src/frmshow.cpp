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


  class OrientationsLayout {
  public:
    OrientationsLayout(const frm::Dimensions dimensions) noexcept : _cell_dimensions(dimensions) {
    }

    sdl::Dimensions dimensions() const noexcept {
      static const unsigned rows = 3;
      static const unsigned cols = 2;

      auto width = this->_cell_dimensions.width * cols;
      auto height = this->_cell_dimensions.height * rows;
      return { width, height };
    }

    sdl::Point cell_pos(frm::Orientation orientation) const {
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

  sdl::StaticTexture create_texture(sdl::Window& w, const pal::File& palette, const frm::Frame& frame) {
    sdl::PixelBuf pixelbuf(frame.dimensions());
    const auto& color_indices = frame.color_indices();
    for (size_t pixel = 0; pixel < color_indices.size(); ++pixel) {
      uint8_t palette_idx = color_indices[pixel];
      pixelbuf[pixel] = palette.palette[palette_idx] * 4;
    }

    return w.mk_static_texture(pixelbuf);
  }

  void show(const pal::File& palette, const frm::Frame& frame) {
    sdl::Context c;
    sdl::Window w = c.create_window(frame.dimensions());

    sdl::StaticTexture t = create_texture(w, palette, frame);

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

  void show(const pal::File& palette, const frm::Animation& animation) {
    sdl::Context c;
    sdl::Window w = c.create_window(animation.dimensions());

    std::vector<sdl::StaticTexture> frame_textures;
    frame_textures.reserve(animation.num_frames());
    for (const frm::Frame& frame : animation.frames()) {
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

  void show(const pal::File& palette, const frm::Orientable& orientable) {
    OrientationsLayout layout{orientable.max_dimensions()};

    sdl::Context c;
    sdl::Window w = c.create_window(layout.dimensions());

    w.render_clear();

    for (frm::Orientation orientation : frm::orientations) {
      const frm::Frame& img = orientable.frame_at(orientation);
      sdl::StaticTexture t = create_texture(w, palette, img);
      sdl::Rect destination{ layout.cell_pos(orientation), img.dimensions() };
      w.render_copy(t, destination);
    }

    w.render_present();

    SDL_Event e;
    while (c.wait_for_event(&e)) {
      if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
        break;
      }
    }
  }

  void show(const pal::File& palette, const frm::OrientableAnimation& am) {
    sdl::Context c;
    const OrientationsLayout layout{am.max_dimensions()};
    sdl::Window w = c.create_window(layout.dimensions());

    std::array<std::vector<sdl::StaticTexture>, frm::num_orientations> frames_per_orient;
    for (frm::Orientation o : frm::orientations) {
      const frm::Animation&  animation = am.animation_at(o);
      std::vector<sdl::StaticTexture>& frames = frames_per_orient[o];
      frames.reserve(am.num_frames_per_orientation());
      for (const frm::Frame& frame : animation.frames()) {
        frames.emplace_back(create_texture(w, palette, frame));
      }
    }

    const std::chrono::milliseconds sleep_dur(1000/am.fps());
    const size_t num_frames = am.num_frames_per_orientation();

    SDL_Event e;
    int frame = 0;

    while (true) {
      while (c.poll_event(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
          return;
        }
      }
      w.render_clear();
      for (frm::Orientation o : frm::orientations) {
        const sdl::Rect destination{layout.cell_pos(o), am.animation_at(o).dimensions() };
        const sdl::StaticTexture& t = frames_per_orient[o][frame % num_frames];
        w.render_copy(t, destination);
      }
      frame++;

      w.render_present();
      sdl::sleep(sleep_dur);
    }        
  }

  void show(const pal::File& palette, const frm::Any& any) {
    switch (any.type()) {
    case frm::AnyType::frame:
      show(palette, any.frame_unpack());
      break;
    case frm::AnyType::animation:
      show(palette, any.animation_unpack());
      break;
    case frm::AnyType::orientable:
      show(palette, any.orientable_unpack());
      break;
    case frm::AnyType::orientable_animation:
      show(palette, any.orientable_animation_unpack());
      break;
    }
  }

  void show(const pal::File& palette, std::istream& frm_strm, const std::string& data_name) {
    try {
      frm::Any any = frm::read_any(frm_strm);
      show(palette, any);
    } catch (const std::exception& ex) {
      throw std::runtime_error(data_name + ": error showing frm: " + ex.what());
    }
  }

  void show(const pal::File& palette, const std::string& frm_pth) {
    if (frm_pth == stdin_senteniel) {
      show(palette, std::cin, "stdin");
    } else {
      std::fstream frm_strm;
      frm_strm.open(frm_pth, std::ios::in | std::ios::binary);

      if (!frm_strm.good()) {
        throw std::runtime_error(std::string(frm_pth) + ": error when opening frm file");
      }

      show(palette, frm_strm, frm_pth);
    }
  }

  void show(const pal::File& palette, const std::vector<std::string>& frm_pths) {
    for (const std::string& frm_pth : frm_pths) {
      show(palette, frm_pth);
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
      show(palette, stdin_senteniel);
    } else {
      show(palette, std::vector<std::string>{ argv + 2, argv + argc });
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": " << ex.what() << std::endl;
    return 1;
  }
}
