/*
 * Filename: gui.c
 * Author:   Jan Faigl, Oleh Borys
 */

#include <SDL.h>
#include <SDL_image.h>
#include "xwin_sdl.h"

#include "utils.h"
#include "computation.h"

#include "event_queue.h"

#include "gui.h"

#ifndef SDL_EVENT_POLL_WAIT_MS
#define SDL_EVENT_POLL_WAIT_MS 10
#endif

struct {
    int w;
    int h;
    unsigned char *img;

} gui = { .img = NULL };

void gui_init(void) {
    get_grid_size(&gui.w, &gui.h);
    gui.img = my_alloc(gui.w * gui.h * 3);
    reset_gui(); // Add this line to set the screen to black
    my_assert(xwin_init(gui.w, gui.h) == 0, __func__, __LINE__, __FILE__);
}
void gui_cleanup(void) {
    if (gui.img) {
        free(gui.img);
        gui.img = NULL;
    }
    xwin_close();
}

void gui_refresh(void) {
    if (gui.img) {
        update_image(gui.w, gui.h, gui.img);
        xwin_redraw(gui.w, gui.h, gui.img);
    }
}

void* gui_win_thread(void* d) {
    info("gui_win_thread - start");
    bool quit = false;
    SDL_Event event;
    struct event ev;
    while (!quit) {
        ev.type = EV_TYPE_NUM;
        if (SDL_PollEvent(&event)) { // rewrite this zanoreni
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_q:
                    ev.type = EV_QUIT;
                    // problem: it still reads char because ReadPipe is blocked
                    break;
                case SDLK_a:
                    ev.type = EV_ABORT;
                    break;
                case SDLK_c:
                    ev.type = EV_COMPUTE;
                    break;
                case SDLK_g:
                    ev.type = EV_GET_VERSION;
                    break;
                case SDLK_r:
                    ev.type = EV_RESET;
                    break;
                default: // added default, nothing chaned I think
                    break;
                }
            }
        } // end SDL event
        if (ev.type != EV_TYPE_NUM) {
            queue_push(ev);
        }
        SDL_Delay(SDL_EVENT_POLL_WAIT_MS);
        // quit is already mutexed
        quit = is_quit();
    }
    info("gui_win_thread - end");
    return NULL;
}

void reset_gui(void) {
    if (gui.img) {
        memset(gui.img, 0, gui.w * gui.h * 3);
        // gui.img = NULL;
    }
}
// Function to clear the window by filling it with a specified color
// void clearWindow(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b) {
//     SDL_SetRenderDrawColor(renderer, r, g, b, 255);
//     SDL_RenderClear(renderer);
//     SDL_RenderPresent(renderer);
// }

void gui_save_image(void) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(gui.img, gui.w, gui.h, 24, gui.w * 3, 0x0000FF, 0x00FF00, 0xFF0000, 0);
    if (surface == NULL) {
        fprintf(stderr, "Failed to create SDL surface: %s\n", SDL_GetError());
        return;
    }

    static int frame_count = 0; // frame counter
    char filename[256];
    snprintf(filename, sizeof(filename), "frames/frame%04d.png", frame_count++);
    IMG_SavePNG(surface, filename); // save the SDL_Surface as PNG

    SDL_FreeSurface(surface);
}

/* end of gui.c */