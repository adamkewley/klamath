#include <iostream>
#include <lib/Mmap.h>
#include <frm/FrmFrame.h>
#include <frm/FrmHeader.h>
#include <frm/FrmParser.h>
#include "FrmDumpFirstFrame.h"
#include <pal/PalFile.h>
#include <pal/PalParser.h>
#include <SDL2/SDL.h>


namespace {
    using namespace klamath;

    void show_bitmap(
            const std::vector<uint8_t>& color_indices,
            const PalFile& pal_file,
            uint16_t w,
            uint16_t h) {

        SDL_Init(SDL_INIT_VIDEO);

        SDL_Renderer *renderer;
        SDL_Window *window;

        if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, &window, &renderer) == -1) {
            throw std::runtime_error(SDL_GetError());
        }

        // First step in SDL2: create a surface (software pixel buffer) and populate that

        SDL_Surface* sdl_surface = SDL_CreateRGBSurface(0, w, h,32,
                0xff000000,  // rgba
                0x00ff0000,
                0x0000ff00,
                0x000000ff);

        uint32_t num_pixels = w * h;

        SDL_LockSurface(sdl_surface);
        for (size_t i = 0; i < num_pixels; i++) {
            uint8_t color_index = color_indices.at(i);
            RgbTriple rgb = pal_file.palette.at(color_index);
            ((Uint32*)sdl_surface->pixels)[i] = SDL_MapRGB(sdl_surface->format, rgb.r * 4 , rgb.g * 4, rgb.b * 4);
        }
        SDL_UnlockSurface(sdl_surface);


        // Second step: create static texture from surface, to be stored in VRAM

        SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, w, h);
        SDL_UpdateTexture(texture, NULL, sdl_surface->pixels, sdl_surface->pitch);  // upload to VRAM
        SDL_FreeSurface(sdl_surface);  // no need for software pixels anymore

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);  // copy VRAM texture into back-buffer
        SDL_RenderPresent(renderer);  // present back-buffer

        SDL_Event e;
        while (SDL_WaitEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                //SDL_DestroyTexture(texture);
                SDL_Quit();
            }
        }
    }
}

int klamath::frm_dump_first_frame_main(int argc, const char **argv) {
    if (argc == 0) {
        std::cerr << "provide pal file as arg" << std::endl;
        return 1;
    } else {
        std::string filename(argv[0]);
        Mmap m = Mmap::from_file(filename);

        FrmHeader frm_header = frm_parse_header(m.get(), m.size());
        uint32_t offset = frm_header.offsets_in_frame_data.at(0);

        FrmFrame frm_frame = frm_parse_frame(m.get() + 0x003e, m.size() - 0x003e);


        Mmap pal_mmap = Mmap::from_file("/tmp/tmp.8YNyk2HKsB/color.pal");
        PalFile pal_file = pal_parse(pal_mmap.get(), pal_mmap.size());

        show_bitmap(frm_frame.color_indices, pal_file, frm_frame.width, frm_frame.height);

        return 0;
    }
}
