#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <SDL2/SDL_events.h>

#include "klamath/rgb.hpp"

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;

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

    class Window {
    public:
      Window(uint32_t width, uint32_t height);
      Window(const Window& other) = delete;
      Window operator=(const Window& other) = delete;
      Window(Window&& tmp);
      ~Window() noexcept;
      
      Texture create_texture(const std::vector<klmth::Rgb>& pixels,
			     size_t width,
			     size_t height);

      void render_clear();
      void render_copy_fullscreen(const Texture& tex);
      void render_present();

    private:
      SDL_Window* w;
      SDL_Renderer* r;
    };

    class Context {
    public:
      Context();
      ~Context() noexcept;

      Window create_window(uint32_t width, uint32_t height);
      bool wait_for_event(SDL_Event* out);
      bool poll_event(SDL_Event* out);
    };

    void sleep(std::chrono::milliseconds duration);
  }
}
