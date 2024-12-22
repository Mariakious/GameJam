#ifndef _NG_GAME_H
#define _NG_GAME_H

// Including this line here since it will
// always be included by main anyways
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef void (*event_handler_t) (SDL_Event*);
typedef void (*render_handler_t) (float delta);

// Just a wrapper around the most basic components
// Can be extended later on and gain more power
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    // Function pointers to constructor the game loop
    event_handler_t handle_event;
    render_handler_t handle_render;

    bool is_running;
    int width, height;
    // Last time the frame was run
    uint32_t last_time;
} ng_game_t;

void ng_game_create(ng_game_t *game, const char *title, int width, int height);
void ng_game_start_loop(ng_game_t *game, event_handler_t ev, render_handler_t re);

void ng_game_destroy(ng_game_t *game);

#endif
