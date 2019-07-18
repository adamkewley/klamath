#include "src/utils/sdl.hpp"

#include <stdexcept>
#include <sstream>

#include <SDL2/SDL.h>


using namespace klmth;

namespace {
  using namespace klmth::sdl;
  using klmth::geometry::area;

  struct Rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;
  };

  SDL_Rect to_sdl_rect(const Rect& rect) {
    SDL_Rect ret;
    ret.x = rect.location.x;
    ret.y = rect.location.y;
    ret.w = rect.dimensions.width;
    ret.h = rect.dimensions.height;
    return ret;
  }
  
  Uint32 to_pixel(SDL_Surface* s, Rgba rgb) {
    return SDL_MapRGBA(s->format, rgb.r, rgb.g, rgb.b, rgb.a);
  }

  class SurfaceLock {
  public:
    SurfaceLock(SDL_Surface* _s) {
      if (SDL_LockSurface(_s) == -1) {
        std::stringstream err;
        err << "could not acquire lock for SDL surface: " << SDL_GetError();
        throw std::runtime_error(err.str());
      } else {
        this->s = _s;
      }
    }

    SurfaceLock(SurfaceLock&& tmp) noexcept {
      this->s = tmp.s;
      tmp.s = nullptr;
    }

    ~SurfaceLock() noexcept {
      if (this->s != nullptr) {
        SDL_UnlockSurface(this->s);
      }
    }

    SDL_Surface* get() const noexcept {
      return this->s;
    }

    void assign(const std::vector<Rgba>& pixels) {
      unsigned area = this->s->w * this->s->h;
      if (pixels.size() > area) {
        throw std::runtime_error("tried to assign outside pixel buffer boundaries");
      } else {
        Uint32* buf = reinterpret_cast<Uint32*>(s->pixels);
        for (size_t i = 0; i < pixels.size(); ++i) {
          buf[i] = to_pixel(this->s, pixels[i]);
        }
      }
    }
  private:
    SDL_Surface* s;
  };

  class Surface {
  public:
    Surface(Dimensions dimensions) {
      this->s = SDL_CreateRGBSurface(0,
                                     dimensions.width,
                                     dimensions.height,
                                     32,
                                     0xff000000,  // rgba
                                     0x00ff0000,
                                     0x0000ff00,
                                     0x000000ff);

      if (this->s == NULL) {
        throw std::runtime_error("null returned when creating SDL surface: out of memory?");
      }
    }
    
    Surface(Dimensions dimensions, const std::vector<Rgba>& pixels) {
      if (geometry::area(dimensions) != pixels.size()) {
        std::stringstream err;
        err << "number of pixels ("  << pixels.size() << ")"
            << " does not match the dimensions of the surface (w = " << dimensions.width
            << " h = " << dimensions.height << ")";
        throw std::runtime_error(err.str());
      }
  
      this->s = SDL_CreateRGBSurface(0,
                                     dimensions.width,
                                     dimensions.height,
                                     32,
                                     0xff000000,  // rgba
                                     0x00ff0000,
                                     0x0000ff00,
                                     0x000000ff);

      if (this->s == NULL) {
        throw std::runtime_error("null returned when creating SDL surface: out of memory?");
      } else {
        SurfaceLock l{this->s};
        l.assign(pixels);
      }
    }

    Surface(const Surface& other) = delete;
    ~Surface() noexcept {
      if (this->s != NULL) {
        SDL_FreeSurface(s);
        this->s = NULL;
      }
    }

    Surface operator=(const Surface& other) = delete;

    void blit(const Surface& src, Rect srcrect, Rect dstrect) {
      SDL_Rect src_sdlrect = to_sdl_rect(srcrect);
      SDL_Rect dst_sdlrect = to_sdl_rect(dstrect);
      if (SDL_BlitSurface(src.s, &src_sdlrect, this->s, &dst_sdlrect) != 0) {
        std::stringstream err;
        err << "error blitting surface: " << SDL_GetError();
        throw std::runtime_error(err.str());
      }
    }
    
    const SDL_Surface* get() const noexcept {
      return this->s;
    }
    
  private:
    SDL_Surface* s;
  };

  StaticTexture mk_texture(SDL_Renderer* r, Dimensions dimensions) {
    SDL_Texture* t = SDL_CreateTexture(r,
                                       SDL_PIXELFORMAT_RGBA8888,
                                       SDL_TEXTUREACCESS_STATIC,
                                       dimensions.width,
                                       dimensions.height);

    if (t == NULL) {
      throw std::runtime_error("cannot create texture");
    } else {
      return {t};
    }
  }

  void render_copy(SDL_Renderer *r, const StaticTexture& texture, const Rect& destination) {
    SDL_Rect sdl_dest = to_sdl_rect(destination);
    if (SDL_RenderCopy(r, texture._t, NULL, &sdl_dest) == -1) {
      throw std::runtime_error("Error copying texture to render target (i.e. the window)");
    }
  }

  StaticTexture mk_static_texture(SDL_Renderer *r,
                                  Dimensions dimensions,
                                  const std::vector<Rgba>& pixels) {
    Surface s{dimensions, pixels};
    StaticTexture t = mk_texture(r, dimensions);
    SDL_UpdateTexture(t._t, NULL, s.get()->pixels, s.get()->pitch);
    return t;
  }

  Rgba to_screen_rgba(pal::Rgb rgb) {
    constexpr uint8_t white = 255;
    constexpr auto frm_brightness_scalar = 4;
    constexpr uint8_t opaque_alpha = 255;
    constexpr Rgba transparent_color{ 0, 0, 0, 0 };
    
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

  StaticTexture create_texture(SDL_Renderer *r,
                               const pal::File& palette,
                               const frm::Frame& frame) {
    std::vector<Rgba> pixels(area(frame));

    const auto& color_indices = frame.data();
    for (auto i = 0U; i < pixels.size(); ++i) {
      uint8_t palette_idx = color_indices[i];
      pixels[i] = to_screen_rgba(palette.palette[palette_idx]);
    }

    return mk_static_texture(r, frame.dimensions(), pixels);
  }

  Point framepos(const Animation& animation, const Frame& frame) {
    frm::Dimensions a = animation.dimensions;
    frm::Dimensions f = frame.dimensions;
    frm::PixelShift shift = frame.shift;

    unsigned x = (a.width/2 - f.width/2) + shift.x;
    unsigned y = (a.height/2 - f.height/2) + shift.y;

    return { x, y };    
  }
}



sdl::StaticTexture::StaticTexture(SDL_Texture* t) noexcept : _t{t} {
}

sdl::StaticTexture::StaticTexture(StaticTexture&& tmp) noexcept {
  this->_t = tmp._t;
  tmp._t = nullptr;
}

sdl::StaticTexture::~StaticTexture() noexcept {
  if (_t != nullptr) {
    SDL_DestroyTexture(_t);
  }
}

sdl::StaticTexture& sdl::StaticTexture::operator=(sdl::StaticTexture&& tmp) noexcept {
      this->_t = tmp._t;
      tmp._t = nullptr;
      return *this;
}

sdl::Window::Window(Dimensions dimensions) {
  if (SDL_CreateWindowAndRenderer(dimensions.width, dimensions.height, SDL_WINDOW_SHOWN, &w, &r) == -1) {
    throw std::runtime_error(std::string("error creating window: ") + SDL_GetError());
  }
}

sdl::Window::Window(Window&& tmp) {
  this->w = tmp.w;
  this->r = tmp.r;
  tmp.w = nullptr;
  tmp.r = nullptr;
}

void sdl::Window::render_clear() {
  // todo: error handling
  SDL_RenderClear(r);
}

void sdl::Window::render_present() {
  // todo: error handling
  SDL_RenderPresent(r);
}

sdl::Window::~Window() noexcept {
  if (r != nullptr) {
    SDL_DestroyRenderer(r);
  }
  if (w != nullptr) {
    SDL_DestroyWindow(w);
  }
}

sdl::Animation sdl::Window::animation(const pal::File& palette, const frm::Animation& frm) {  
  std::vector<Frame> frames;
  for (const frm::Frame& frame : frm.frames()) {
    Frame f{create_texture(this->r, palette, frame),
            frame.dimensions(),
            frame.pixel_shift()};
    frames.emplace_back(std::move(f));
  }
  
  return { std::move(frames), frm.dimensions() };
}

sdl::StaticTexture sdl::Window::texture(const pal::File& palette, const frm::Frame& img) {
    return create_texture(this->r, palette, img);
}
      
void sdl::Window::render_frame(const Animation& anim, size_t framenum, const Rect& destination) {
  const Frame& f = anim.frames[framenum];
  Point pos = destination.location + framepos(anim, f);
  
  render_copy(this->r, f.texture, { pos, f.dimensions });
}

void sdl::Window::render_texture(StaticTexture& texture, const Rect& destination) {
    render_copy(this->r, texture, destination);
}

sdl::Context::Context() {
  SDL_Init(SDL_INIT_VIDEO);
}

sdl::Window sdl::Context::create_window(Dimensions dimensions) {
  return sdl::Window(dimensions);
}

bool sdl::Context::wait_for_event(SDL_Event* out) {
  return SDL_WaitEvent(out) == 1;
}

bool sdl::Context::poll_event(SDL_Event* out) {
  return SDL_PollEvent(out) == 1;
}

sdl::Context::~Context() noexcept {
  SDL_Quit();
}
