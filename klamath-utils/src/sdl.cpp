#include "src/sdl.hpp"

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
    Surface(uint32_t width, uint32_t height) {
      s = SDL_CreateRGBSurface(0,
			       width,
			       height,
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

    const void* get_pixels() const {
      return s->pixels;
    }

    int get_pitch() const {
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


  klmth::sdl::Texture mk_texture(SDL_Renderer* r, size_t width, size_t height) {
    SDL_Texture* t = SDL_CreateTexture(r,
				       SDL_PIXELFORMAT_RGBA8888,
				       SDL_TEXTUREACCESS_STATIC,
				       width,
				       height);
      
    if (t == NULL) {
      throw std::runtime_error("cannot create texture");
    } else {
      return {t};
    }
  }  
}



klmth::sdl::Texture::Texture(SDL_Texture* t) : _t(t) {
}

klmth::sdl::Texture::Texture(Texture&& tmp) {
  this->_t = tmp._t;
  tmp._t = nullptr;
}

klmth::sdl::Texture::~Texture() noexcept {
  if (_t != nullptr) {
    SDL_DestroyTexture(_t);
  }
}



klmth::sdl::Window::Window(uint32_t width, uint32_t height) {
  if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, &w, &r) == -1) {
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

void klmth::sdl::Window::render_copy_fullscreen(const Texture& texture) {
  if (SDL_RenderCopy(r, texture._t, NULL, NULL) == -1) {
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

klmth::sdl::Texture klmth::sdl::Window::create_texture(const std::vector<klmth::Rgb>& pixels,
						       size_t width,
						       size_t height) {
  Surface s(width, height);
  {
    SurfaceLock l = s.lock();
    l.assign_pixel_range(0, width * height, [&](size_t i) {
	return s.to_pixel(pixels[i]);
      });
  }

  Texture t = mk_texture(r, width, height);
  SDL_UpdateTexture(t._t, NULL, s.get_pixels(), s.get_pitch());

  return t;
}



klmth::sdl::Context::Context() {
  SDL_Init(SDL_INIT_VIDEO);
}

klmth::sdl::Window klmth::sdl::Context::create_window(uint32_t width, uint32_t height) {
  return klmth::sdl::Window(width, height);
}

bool klmth::sdl::Context::wait_for_event(SDL_Event* out) {
  return SDL_WaitEvent(out) == 1;
}

klmth::sdl::Context::~Context() noexcept {
  SDL_Quit();
}