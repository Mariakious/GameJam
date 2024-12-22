#include "game.h"
#include "common.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>

// You might want to change that!
#define FPS 60
#define IDEAL_MS_PER_FRAME (1000.0 / FPS)

void ng_game_create(ng_game_t *game, const char *title, int width, int height)
{
    // Provide the randomness generator with a unique seed
    srand(time(NULL));
    
    // Initializing SDL components
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        ng_die("failed to initialize SDL2");
    
    if (IMG_Init(IMG_INIT_PNG) < 0)
        ng_die("failed to initialize SDL2/SDL_image");

    if (TTF_Init() < 0)
        ng_die("failed to initialize SDL2/SDL_ttf");

    // Initializing SDL_mixer with the standard settings
#ifndef NO_AUDIO
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        ng_die("failed to open audio device and initialize SDL_Mixer");
#endif
    
    game->width = width;
    game->height = height;
    
    // Creating the window at the center of the screen with the specified properties
    game->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    width, height, SDL_WINDOW_SHOWN);

    if (!game->window)
        ng_die("failed to create the default SDL2 window");

    // -1: Initialize the first available rendering GPU driver
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

    game->is_running = true;
}

static void main_game_loop(void *args)
{
    // The argument will always be an ng_game_t* pointer
    // The signature is defined like this just for the sake of emscripten
    ng_game_t *game = args;

    if (!game->is_running)
    {
        ng_game_destroy(game);

    #ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
    #else
        exit(EXIT_SUCCESS);
    #endif
    }

    // Calculate the amount of seconds that passed since the last frame
    uint32_t cur_time = SDL_GetTicks();
    float delta = (cur_time - game->last_time) / 1000.0f;
    game->last_time = cur_time;

    static SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // SDL_QUIT = the window is about to close, for whatever
        // reason (exit button, alt f4 etc)
        if (event.type == SDL_QUIT)
            game->is_running = false;
        else
            game->handle_event(&event);
    }

    SDL_SetRenderDrawColor(game->renderer, 10, 10, 10, 255);
    SDL_RenderClear(game->renderer);

    game->handle_render(delta);

    // Sends the instructions into our GPU, updates the screen
    SDL_RenderPresent(game->renderer);

    // Don't update too fast, introduce an FPS limit!
    // This is an important performance measure, since
    // updating faster is pointless! The frequency is too fast
    // for it to ever be visible on the monitor
    if (delta < IDEAL_MS_PER_FRAME)
        SDL_Delay(IDEAL_MS_PER_FRAME - delta);
}

void ng_game_start_loop(ng_game_t *game, event_handler_t ev, render_handler_t re)
{
    game->handle_event = ev;
    game->handle_render = re;

#ifdef __EMSCRIPTEN__
    // If we're running on the web, we need to wrap around emscripten
    emscripten_set_main_loop_arg(main_game_loop, game, FPS, true);
#else
    for (;;) main_game_loop(game);
#endif
}

// Clearing up all SDL components
void ng_game_destroy(ng_game_t *game)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
#ifndef NO_AUDIO
    Mix_Quit();
#endif
}
