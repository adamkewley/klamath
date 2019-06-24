#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <chrono>

#include "src/utils/geometry.hpp"

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
    
    class StaticTexture {
    public:
      StaticTexture(SDL_Texture* t) noexcept;
      StaticTexture(const StaticTexture& other) = delete;
      StaticTexture(StaticTexture&& tmp) noexcept;
      ~StaticTexture() noexcept;

      StaticTexture operator=(const StaticTexture& other) = delete;

    private:
      SDL_Texture* _t;
      friend class Window;
    };

    struct Rgba {
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t a = 255;
    };

    class Surface {
    public:
      Surface(Dimensions dimensions);
      Surface(Dimensions dimensions, const std::vector<Rgba>& pixels);
      Surface(const Surface& other) = delete;
      ~Surface() noexcept;

      Surface operator=(const Surface& other) = delete;

      void blit(const Surface& src, Rect srcrect, Rect dstrect);
      const SDL_Surface* get() const noexcept;

    private:
      SDL_Surface* s;
    };
    

    class Window {
    public:
      Window(Dimensions dimensions);
      Window(const Window& other) = delete;
      Window(Window&& tmp);
      ~Window() noexcept;

      Window operator=(const Window& other) = delete;

      StaticTexture mk_static_texture(Dimensions dimensions,
                                      const std::vector<Rgba>& pixels);

      StaticTexture mk_static_texture(Dimensions dimensions,
                                      const std::vector<Rgba>& pixels,
                                      const Rect& target);

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
  }
}
