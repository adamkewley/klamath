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
union SDL_Event;

namespace klmth {
  namespace sdl {

    class Texture {
    public:
      Texture(SDL_Texture* t);
      Texture(const Texture& other) = delete;
      Texture(Texture&& tmp);
      ~Texture() noexcept;

      SDL_Texture* _t;  // todo: make private
    };

    using Dimensions = geometry::Dimensions<uint32_t>;
    using Point = geometry::Point<unsigned>;
    using Rect = geometry::Rect<uint32_t, unsigned>;

    class Window {
    public:
      Window(Dimensions dimensions);
      Window(const Window& other) = delete;
      Window operator=(const Window& other) = delete;
      Window(Window&& tmp);
      ~Window() noexcept;

      Texture create_texture(const std::vector<klmth::Rgb>& pixels,
                             Dimensions dimensions);

      void render_clear();
      void render_copy_fullscreen(const Texture& texture);
      void render_copy(const Texture& texture, const Rect& destination);
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
