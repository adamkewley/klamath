#include "src/utils/sdl.hpp"

#include <stdexcept>
#include <sstream>

#include <SDL2/SDL.h>

namespace {
  using namespace klmth::sdl;

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

  SDL_Rect to_sdl_rect(const Rect& rect) {
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

klmth::sdl::Surface::Surface(Dimensions dimensions) {
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

klmth::sdl::Surface::Surface(Dimensions dimensions,
                             const std::vector<Rgba>& pixels) {
  
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

klmth::sdl::Surface::~Surface() noexcept {
  if (this->s != NULL) {
    SDL_FreeSurface(s);
    this->s = NULL;
  }
}

const SDL_Surface* klmth::sdl::Surface::get() const noexcept {
  return this->s;
}

void klmth::sdl::Surface::blit(const Surface& src, Rect srcrect, Rect dstrect) {
  SDL_Rect src_sdlrect = to_sdl_rect(srcrect);
  SDL_Rect dst_sdlrect = to_sdl_rect(dstrect);
  if (SDL_BlitSurface(src.s, &src_sdlrect, this->s, &dst_sdlrect) != 0) {
    std::stringstream err;
    err << "error blitting surface: " << SDL_GetError();
    throw std::runtime_error(err.str());
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

klmth::sdl::StaticTexture klmth::sdl::Window::mk_static_texture(Dimensions dimensions,
                                                                const std::vector<Rgba>& pixels) {
  Surface s{dimensions, pixels};
  StaticTexture t = mk_texture(r, dimensions);
  SDL_UpdateTexture(t._t, NULL, s.get()->pixels, s.get()->pitch);
  return t;
}

klmth::sdl::StaticTexture klmth::sdl::Window::mk_static_texture(Dimensions dimensions,
                                                                const std::vector<Rgba>& pixels,
                                                                const Rect& target) {
  Surface src{dimensions, pixels};
  Surface targ{target.dimensions};

  targ.blit(src, { {0, 0}, dimensions }, target);

  StaticTexture t = mk_texture(this->r, target.dimensions);

  SDL_UpdateTexture(t._t, NULL, targ.get()->pixels, targ.get()->pitch);

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
