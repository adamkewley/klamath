#include "src/utils/sdl.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>

namespace {

  class SurfaceLock {
  public:
    SurfaceLock(SDL_Surface* s) {
      if (SDL_LockSurface(s) == -1) {
        throw std::runtime_error("could not acquire SDL surface lock");
      } else {
        _s = s;
        _n_pix = static_cast<size_t>(s->w * s->h);
      }
    }

    SurfaceLock(const SurfaceLock& other) = delete;

    SurfaceLock(SurfaceLock&& tmp) {
      this->_s = tmp._s;
      this->_n_pix = tmp._n_pix;
      tmp._s = nullptr;
    }

    template<typename ValueGetter>
    void assign_pixel_range(size_t offset, size_t n, ValueGetter f) {
      size_t end = n + offset;
      if (end > _n_pix) {
        throw std::runtime_error("tried to assign outside pixel buffer boundaries");
      } else {
        Uint32* pixels = reinterpret_cast<Uint32*>(_s->pixels);
        for (size_t i = offset; i < end; ++i) {
          pixels[i] = f(i);
        }
      }
    }

    ~SurfaceLock() noexcept {
      if (_s != nullptr) {
        SDL_UnlockSurface(_s);
      }
    }
  private:
    SDL_Surface* _s;
    size_t _n_pix;
  };


  class Surface {
  public:
    Surface(klmth::sdl::Dimensions dimensions) {
      s = SDL_CreateRGBSurface(0,
                               dimensions.width,
                               dimensions.height,
                               32,
                               0xff000000,  // rgba
                               0x00ff0000,
                               0x0000ff00,
                               0x000000ff);

      if (s == NULL) {
        throw std::runtime_error("null returned when creating SDL surface: out of memory?");
      }
    }

    SurfaceLock lock() {
      return SurfaceLock(s);
    }

    Uint32 to_pixel(const klmth::Rgb& rgb) {
      return SDL_MapRGB(s->format, rgb.r, rgb.g, rgb.b);
    }

    const void* pixels() const {
      return s->pixels;
    }

    int pitch() const {
      return s->pitch;
    }

    ~Surface() noexcept {
      if (s != NULL) {
        SDL_FreeSurface(s);
      }
    }

  private:
    SDL_Surface* s;
  };


  klmth::sdl::StaticTexture mk_texture(SDL_Renderer* r, klmth::sdl::Dimensions dimensions) {
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

  SDL_Rect to_sdl_rect(const klmth::sdl::Rect& rect) {
    SDL_Rect ret;
    ret.x = rect.location.x;
    ret.y = rect.location.y;
    ret.w = rect.dimensions.width;
    ret.h = rect.dimensions.height;
    return ret;
  }
}



klmth::sdl::StaticTexture::StaticTexture(SDL_Texture* t) noexcept : _t(t) {
}

klmth::sdl::StaticTexture::StaticTexture(StaticTexture&& tmp) noexcept {
  this->_t = tmp._t;
  tmp._t = nullptr;
}

klmth::sdl::StaticTexture::~StaticTexture() noexcept {
  if (_t != nullptr) {
    SDL_DestroyTexture(_t);
  }
}

klmth::sdl::Window::Window(Dimensions dimensions) {
  if (SDL_CreateWindowAndRenderer(dimensions.width, dimensions.height, SDL_WINDOW_SHOWN, &w, &r) == -1) {
    throw std::runtime_error(std::string("error creating window: ") + SDL_GetError());
  }
}

klmth::sdl::Window::Window(Window&& tmp) {
  this->w = tmp.w;
  this->r = tmp.r;
  tmp.w = nullptr;
  tmp.r = nullptr;
}

void klmth::sdl::Window::render_clear() {
  // todo: error handling
  SDL_RenderClear(r);
}

void klmth::sdl::Window::render_copy_fullscreen(const StaticTexture& texture) {
  if (SDL_RenderCopy(r, texture._t, NULL, NULL) == -1) {
    throw std::runtime_error("Error copying texture to render target (i.e. the window)");
  }
}

void klmth::sdl::Window::render_copy(const StaticTexture& texture, const Rect& destination) {
  SDL_Rect sdl_dest = to_sdl_rect(destination);
  if (SDL_RenderCopy(r, texture._t, NULL, &sdl_dest) == -1) {
    throw std::runtime_error("Error copying texture to render target (i.e. the window)");
  }
}

void klmth::sdl::Window::render_present() {
  // todo: error handling
  SDL_RenderPresent(r);
}

klmth::sdl::Window::~Window() noexcept {
  if (r != nullptr) {
    SDL_DestroyRenderer(r);
  }
  if (w != nullptr) {
    SDL_DestroyWindow(w);
  }
}

klmth::sdl::StaticTexture klmth::sdl::Window::mk_static_texture(const PixelBuf& pixelbuf) {
  Surface s(pixelbuf.dimensions());
  {
    SurfaceLock l = s.lock();
    l.assign_pixel_range(0, pixelbuf.size(), [&](size_t i) {
        return s.to_pixel(pixelbuf[i]);
      });
  }

  StaticTexture t = mk_texture(r, pixelbuf.dimensions());
  SDL_UpdateTexture(t._t, NULL, s.pixels(), s.pitch());

  return t;
}



klmth::sdl::Context::Context() {
  SDL_Init(SDL_INIT_VIDEO);
}

klmth::sdl::Window klmth::sdl::Context::create_window(Dimensions dimensions) {
  return klmth::sdl::Window(dimensions);
}

bool klmth::sdl::Context::wait_for_event(SDL_Event* out) {
  return SDL_WaitEvent(out) == 1;
}

bool klmth::sdl::Context::poll_event(SDL_Event* out) {
  return SDL_PollEvent(out) == 1;
}

klmth::sdl::Context::~Context() noexcept {
  SDL_Quit();
}


void klmth::sdl::sleep(std::chrono::milliseconds duration) {
  SDL_Delay(duration.count());
}
