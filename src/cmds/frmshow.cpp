#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <thread>

#include <SDL2/SDL_events.h>

#include "third_party/CLI11.hpp"

#include "src/formats/pal_reader.hpp"
#include "src/formats/frm_reader.hpp"
#include "src/utils/sdl.hpp"

using klmth::geometry::area;
using klmth::geometry::width;
using klmth::geometry::height;


namespace {
  using namespace klmth;

  const std::string stdin_senteniel = "-";


  class OrientationsLayout {
  public:
    OrientationsLayout(const frm::File& f) noexcept {
      for (frm::Orientation o : frm::orientations) {
        frm::Dimensions d = f.animation(o).dimensions();
        this->cell_dims = klmth::geometry::union_of(this->cell_dims, d);
      }
    }

    sdl::Dimensions dimensions() const noexcept {
      return { cols * cell_dims.width, rows * cell_dims.height };
    }

    sdl::Point cell_pos(frm::Orientation orientation) const {
      switch (orientation) {
      case frm::Orientation::north_east:
        return { cell_dims.width, 0 };
      case frm::Orientation::east:
        return { cell_dims.width, cell_dims.height };
      case frm::Orientation::south_east:
        return { cell_dims.width, cell_dims.height * 2u };
      case frm::Orientation::south_west:
        return { 0, cell_dims.height * 2u };
      case frm::Orientation::west:
        return { 0, cell_dims.height };
      case frm::Orientation::north_west:
        return { 0, 0 };
      default:
        throw std::runtime_error("unknown orientation: cannot calculate location for this orientation in the layout");
      }
    }

    frm::Dimensions cell_dims{0, 0};

  private:
    static constexpr unsigned cols = 2;
    static constexpr unsigned rows = 3;
  };
  

  pal::File load_palette(const std::string& source) {
    std::ifstream pal_in{source, std::ios::in | std::ios::binary};

    if (pal_in.bad()) {
      throw std::runtime_error(source + ": error when opening palette (.pal) file");
    }

    try {
      return pal::parse(pal_in);
    } catch (const std::exception& ex) {
      throw std::runtime_error(source + ": error when parsing palette (.pal) file: " + ex.what());
    }
  }

  sdl::Rgba to_screen_rgba(pal::Rgb rgb) {
    constexpr uint8_t white = 255;
    constexpr auto frm_brightness_scalar = 4;
    constexpr uint8_t opaque_alpha = 255;
    constexpr sdl::Rgba transparent_color{ 0, 0, 0, 0 };
    
    if (rgb.r == white &&
        rgb.g == white &&
        rgb.b == white) {
      return transparent_color;
    } else {
      return {
        static_cast<uint8_t>(frm_brightness_scalar * rgb.r),
        static_cast<uint8_t>(frm_brightness_scalar * rgb.g),
        static_cast<uint8_t>(frm_brightness_scalar * rgb.b),
        opaque_alpha,
      };
    }
  }

  sdl::StaticTexture create_texture(sdl::Window& w,
                                    const pal::File& palette,
                                    const frm::Frame& frame) {
    std::vector<sdl::Rgba> pixels(area(frame));

    const auto& color_indices = frame.data();
    for (auto i = 0U; i < pixels.size(); ++i) {
      uint8_t palette_idx = color_indices[i];
      pixels[i] = to_screen_rgba(palette.palette[palette_idx]);
    }

    return w.mk_static_texture(frame.dimensions(), pixels);
  }

  struct SdlAnimFrame {
    sdl::StaticTexture texture;
    frm::Dimensions dimensions;
    frm::PixelShift shift;
  };

  struct SdlAnim {
    std::vector<SdlAnimFrame> frames;
    frm::Dimensions dimensions;
  };

  SdlAnimFrame create_frame(sdl::Window& w,
                            const pal::File& palette,
                            const frm::Frame& frame) {
    sdl::StaticTexture t = create_texture(w, palette, frame);

    return { std::move(t), frame.dimensions(), frame.pixel_shift() };
  }

  SdlAnim create_animation(sdl::Window& w,
                           const pal::File& palette,
                           const frm::Animation& animation) {
    std::vector<SdlAnimFrame> frames;
    for (const frm::Frame& frame : animation.frames()) {
      frames.push_back(create_frame(w, palette, frame));
    }

    return { std::move(frames), animation.dimensions() };
  }

  sdl::Point pos(const SdlAnim& animation, const SdlAnimFrame& frame) {
    frm::Dimensions a = animation.dimensions;
    frm::Dimensions f = frame.dimensions;
    frm::PixelShift shift = frame.shift;

    unsigned x = (a.width/2 - f.width/2) + shift.x;
    unsigned y = (a.height/2 - f.height/2) + shift.y;

    return { x, y };    
  }

  void show(const pal::File& palette, const frm::File& f) {
    const OrientationsLayout layout{f};
    
    sdl::Context c;
    sdl::Window w = c.create_window(layout.dimensions());

    std::array<SdlAnim, frm::num_orientations> anims {{
      { create_animation(w, palette, f.animation(frm::north_east)) },
        { create_animation(w, palette, f.animation(frm::east)) },
          { create_animation(w, palette, f.animation(frm::south_east)) },
            { create_animation(w, palette, f.animation(frm::south_west)) },
              { create_animation(w, palette, f.animation(frm::west)) },
                { create_animation(w, palette, f.animation(frm::north_west)) }
      }};

    const uint16_t fps = f.animation(frm::north_east).fps();
    const std::chrono::milliseconds sleep_dur(fps ? 1000/fps : 500);

    SDL_Event e;
    unsigned framenum = 0;

    while (true) {
      while (c.poll_event(&e)) {
        if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
          return;
        }
      }
      w.render_clear();
      for (frm::Orientation o : frm::orientations) {
        const SdlAnim& anim = anims[o];
        const SdlAnimFrame& frame = anim.frames[framenum % anim.frames.size()];
        sdl::Point framepos = layout.cell_pos(o) + pos(anim, frame);
        sdl::Rect target{ framepos, frame.dimensions };
        
        w.render_copy(frame.texture, target);
      }
      ++framenum;

      w.render_present();
      std::this_thread::sleep_for(sleep_dur);
    }
  }

  void show(const pal::File& palette, std::istream& frm_strm, const std::string& data_name) {
    try {
      frm::File f = frm::read_file(frm_strm);
      show(palette, f);
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

      std::cerr << "frmshow: showing: " << frm_pth << std::endl;
      show(palette, frm_strm, frm_pth);
    }
  }

  void show(const pal::File& palette, const std::vector<std::string>& frm_pths) {
    for (const std::string& frm_pth : frm_pths) {
      show(palette, frm_pth);
    }
  }
}


int cmd_frmshow(int argc, char** argv) {
  CLI::App app{"show frm files in a GUI window"};
  std::string pal_pth;
  app.add_option("pal_pth", pal_pth, "path to a PAL file")->required();
  std::vector<std::string> frm_pths;
  app.add_option("frm_file", frm_pths, "path to FRM files. '-' interpreted as stdin. Supplying no paths will cause it to read FRM data from stdin");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError& ex) {
    return app.exit(ex);
  }

  try {
    pal::File palette = load_palette(pal_pth);

    if (frm_pths.size() == 0) {
      show(palette, stdin_senteniel);
    } else {
      show(palette, frm_pths);
    }

    return 0;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": " << ex.what() << std::endl;
    return 1;
  }
}
