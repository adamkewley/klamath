#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <chrono>

#include "klamath/rgb.hpp"
#include "klamath/geometry.hpp"

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
union SDL_Event;

namespace klmth {
  namespace sdl {

    using Dimensions = geometry::Dimensions<uint32_t>;
    using Point = geometry::Point<unsigned>;
    using Rect = geometry::Rect<uint32_t, unsigned>;
    
    class PixelBuf {
    public:
      PixelBuf(Dimensions dimensions) : _dimensions(dimensions) {
        this->pixels.resize(geometry::area(dimensions));
      }

      const klmth::Rgb& operator[](size_t idx) const {
        return this->pixels[idx];
      }

      klmth::Rgb& operator[](size_t idx) {
        return this->pixels[idx];
      }

      Dimensions dimensions() const noexcept {
        return this->_dimensions;
      }

      size_t size() const noexcept {
        return this->pixels.size();
      }

      void resize(Dimensions new_dimensions) {
        this->pixels.resize(geometry::area(new_dimensions));
        this->_dimensions = new_dimensions;
      }
      
    private:
      std::vector<klmth::Rgb> pixels;
      Dimensions _dimensions;
    };

    class StaticTexture {
    public:
      StaticTexture(SDL_Texture* t) noexcept;
      StaticTexture(const StaticTexture& other) = delete;
      StaticTexture(StaticTexture&& tmp) noexcept;
      ~StaticTexture() noexcept;

    private:
      SDL_Texture* _t;
      friend class Window;
    };

    class Window {
    public:
      Window(Dimensions dimensions);
      Window(const Window& other) = delete;
      Window operator=(const Window& other) = delete;
      Window(Window&& tmp);
      ~Window() noexcept;

      StaticTexture mk_static_texture(const PixelBuf& pixel_buf);

      void render_clear();
      void render_copy_fullscreen(const StaticTexture& texture);
      void render_copy(const StaticTexture& texture, const Rect& destination);
      void render_present();

    private:      
      SDL_Window* w;
      SDL_Renderer* r;
    };

    class Context {
    public:
      Context();
      ~Context() noexcept;

      Window create_window(Dimensions dimensions);
      bool wait_for_event(SDL_Event* out);
      bool poll_event(SDL_Event* out);
    };

    void sleep(std::chrono::milliseconds duration);
  }
}
