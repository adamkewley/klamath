#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <chrono>

#include "src/utils/geometry.hpp"
#include "src/formats/pal.hpp"
#include "src/formats/frm.hpp"

struct SDL_Texture;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
union SDL_Event;

namespace klmth {
  namespace sdl {

    using Dimensions = geometry::Dimensions<unsigned>;
    using Point = geometry::Point<int>;
    using Rect = geometry::Rect<int, unsigned>;
    using Shift = geometry::Point<int16_t>;

    class StaticTexture {
    public:
      StaticTexture(SDL_Texture* t) noexcept;
      StaticTexture(const StaticTexture& other) = delete;
      StaticTexture(StaticTexture&& tmp) noexcept;
      ~StaticTexture() noexcept;

      StaticTexture operator=(const StaticTexture& other) = delete;
      StaticTexture& operator=(StaticTexture&& tmp) noexcept;

      SDL_Texture* _t;
    };

    struct Frame {
      StaticTexture texture;
      Dimensions dimensions;
      Shift shift;
    };

    struct Animation {
      std::vector<Frame> frames;
      Dimensions dimensions;
    };

    class Window {
    public:
      Window(Dimensions dimensions);
      Window(const Window& other) = delete;
      Window(Window&& tmp);
      ~Window() noexcept;

      Window operator=(const Window& other) = delete;

      Animation animation(const pal::File& palette, const frm::Animation& frm);
      StaticTexture texture(const pal::File& palette, const frm::Frame& img);

      void render_frame(const Animation& anim, size_t frame, const Rect& destination);
      void render_texture(StaticTexture& texture, const Rect& destination);
      void render_clear();
      void render_present();

      void set_title(const std::string& t);

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
