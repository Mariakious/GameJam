# Resources

- [Official SDL Wiki](https://wiki.libsdl.org/SDL3/CategoryAPIFunction)
- [Game Programming Patterns (C++)](http://gameprogrammingpatterns.com/)

# What is SDL

> Simple DirectMedia Layer is a cross-platform development library
> designed to provide low level access to audio, keyboard, mouse,
> joystick, and graphics hardware via OpenGL and Direct3D.

Games written using SDL can be trivially exported to multiple
platforms: that's not the case when writing OpenGL code from scratch.

Some commercial engines use SDL as a cross-platform abstractions over
windows and input handling. They implement performance-critical stuff
in OpenGL or Vulkan instead. When working with 2D games though, SDL
might make perfect sense. You might want to check out [this
list](https://libregamewiki.org/SDL_games) of free and open source
games written in SDL.

There's a recent (minority) trend of people abandoning languages
associated with OOP (such as C++), to instead create their games in a
data-oriented approach. This movement is expressed in languages such as
`Odin`, and the upcoming `Jai`. These new languages were created
particularly with game development in mind but are not OOP! They all
strive to be a better C, procedural languages with additional modern
features such as generics and namespaces. Definitely do check them
out! Games like these are implemented using database-like
technologies, such as entity component systems (**ECS**).

Data-driven game development can be faster than traditional OOP, since
it can more effectively guarantee memory locality, among others.
Thinking of game state as big chunks of pure data that need to be
processed is clever!

## Abstracting Code in C!

Header File: `engine/game.h`

```c
#ifndef _NG_GAME_H
#define _NG_GAME_H

#include <SDL2/SDL.h>

/*
 * The game struct will manage and encapsule the two most basic SDL components:
 * the window and the renderer, which is responsible for exploiting
 * hardware acceleration (GPU) to provide basic drawing operations.
 */
typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int width, height;
// No namespaces in C, we need to use prefixes instead
// Suffixing all new types with _t, a common practice
} ng_game_t;

// External interface!
void ng_game_create(ng_game_t *game, const char *title, int width, int height);
void ng_game_destroy(ng_game_t *game);

#endif
```

Implementation File: `engine/game.c`

```c
#include "game.h"
#include "common.h"
#include <SDL2/SDL_image.h>
#include <time.h>

// That's how we simulate object oriented programming in C
// The first argument is a pointer, just like the implicit `this` in C++
// It's a good practice to prefix "struct methods" with the struct's name
void ng_game_create(ng_game_t *game, const char *title, int width, int height)
{
    // Generating a unique seed for our random number generator
    srand(time(NULL));
    
    // Initializing all SDL components
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        ng_die("failed to initialize SDL2");

    // Will be used to load PNG images into SDL textures
    if (IMG_Init(IMG_INIT_PNG) < 0)
        ng_die("failed to initialize SDL2/SDL_image");

    game->width = width;
    game->height = height;
    
    // Creating the window at the center of the screen with the specified properties
    game->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    width, height, SDL_WINDOW_SHOWN);

    if (!game->window)
        ng_die("failed to create the default SDL2 window");
    
    // WARNING: Ignore all SDL tutorials that work with surfaces
    // Hardware acceleration was normalized after SDL2 was first released
    // -1 will select the first rendering driver matching the given flags
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
}

// Clearing up all SDL components
void ng_game_destroy(ng_game_t *game)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);

    IMG_Quit();
    SDL_Quit();
}
```

We will build a set of utility functions that will be accessible to
all components of our simple engine. We should include them all in a
common header file called `engine/common.h`.

```c
// Write that header file yourself!
#include "common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void ng_die(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Just print out the supplied message with an informative prefix
    fprintf(stderr, "{engine fatal} ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    
    va_end(args);
    exit(EXIT_FAILURE);
}

int ng_random_int_in_range(int start, int end)
{
    // Remember (`man 3 rand`): rand() returns an integer in [0, RAND_MAX]
    // RAND_MAX is usually defined as a really large number
    return start + rand() % (end - start + 1);
}

bool ng_random_bool(void)
{
    return rand() % 2;
}
```

## Defining the Game Loop!

```c
#define FPS 60
#define MS_PER_FRAME (1000.0 / FPS)

/*
 * Creating a context to store all our global variables
 * This is a really common pattern in data-oriented game development
 *  it effectively works like a namespace
 */
typedef struct
{
    ng_game_t game;
    bool is_paused;

    bool is_game_running;
} ctx_t;

// Static here refers to linkage, it's not a storage class as you
// might have expected! This means that this variable will only be
// available in this file. The default behavior is identical to that of
// writing `extern`, so be careful!

// TIP: All functions that should not be accessible to other files
// should by prefixed by `static`, otherwise they might clash with each
// other!
static ctx_t ctx;

// Functions that do not accept any arguments should be defined as
// (void). initialize_game() would have been a function that accepts
// **any** number of arguments, so initialize_game(1, 2) would have
// worked just fine, strangely enough...
static void initialize_game(void)
{
    ctx.is_game_running = true;
    ng_game_create(&ctx.game, "Minecraft 2", 320, 320);
}

int main(int argc, char **argv)
{
    initialize_game();

    // or just SDL_Rect example = {10, 10, 100, 100};
    SDL_Rect example = {
        .x = 10, .y = 10,
        .w = 100, .h = 100,
    };

    uint32_t last_time = 0;
    uint32_t delta = 0;
    SDL_Event event;
    
    while (ctx.is_game_running)
    {
        // Calculating the amount of milliseconds that passed
        // since the last execution of this loop. Delta time is
        // important, because some frames might take longer to update
        // and draw than others. That duration should be reflected to
        // the amount by which we update stuff, such as velocities and
        // positions!
        uint32_t cur_time = SDL_GetTicks();
        delta = cur_time - last_time;
        last_time = cur_time;

        // Before trying to render stuff on the screen,
        // we should first check if any new input is available
        while (SDL_PollEvent(&event))
        {
            // SDL_QUIT = the window is about to close, for whatever
            // reason (exit button, alt f4 etc)
            switch (event.type)
            {
                case SDL_QUIT:
                    ctx.is_game_running = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    // Mouse position is accessible from the event struct,
                    // which is efficiently defined to be a C union!
                    if (ng_point_in_rect(&example, event.button.x, event.button.y))
                        example.x += 10;
                    
                    break;
            }
        }

        SDL_SetRenderDrawColor(ctx.game.renderer, 10, 10, 10, 255);
        SDL_RenderClear(ctx.game.renderer);

        SDL_SetRenderDrawColor(ctx.game.renderer, 255, 0, 0, 255);
        // Will use the active render draw color, as defined earlier!
        SDL_RenderFillRect(ctx.game.renderer, &example);

        // Sends the instructions into our GPU, updates the screen
        SDL_RenderPresent(ctx.game.renderer);

        // Don't update too fast, introduce an FPS limit!
        // This is an important performance measure, since
        // updating faster is pointless! The frequency is too fast
        // for it to ever be visible on the monitor
        if (delta < MS_PER_FRAME)
            SDL_Delay(MS_PER_FRAME - delta);
    }

    ng_game_destroy(&ctx.game);
}
```

## Sample Project Structure and Makefile

```
Makefile
.gitignore
README.md
src/
    engine/
        game.h
        game.c
        custom_math.h
        custom_math.c

    enemies.c (or whatever)
    main.c
```

```make
# Returns all c files nested or not in $(1)
define collect_sources
	$(shell find $(1) -name '*.c')
endef

# Modify these variables to apply your preferences
OBJ_DIR := objects
EXE_NAME := bin

SOURCES := $(call collect_sources, src)
OBJECTS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Make sure it is already installed! Consult your package manager
LIBRARIES := sdl2 SDL2_image
C_FLAGS := `pkg-config --cflags $(LIBRARIES)`
L_FLAGS := `pkg-config --libs $(LIBRARIES)` -lm

# Custom targets that do not correspond to real files
.PHONY: run clean

run: $(EXE_NAME)
	@# Executing the program once it has successfully been built
	@./$(EXE_NAME)

$(EXE_NAME): $(OBJECTS)
	@# Lines prefixed with @ will not be displayed in the output of the `make` command
	@# Note however that *their* output, will
	@echo "{Makefile} Creating the executable"
	@$(CC) $(OBJECTS) -o $(EXE_NAME) $(L_FLAGS)

objects/%.o: %.c
	@# Making sure that the directory already exists before creating the object
	@# man mkdir!
	@mkdir -p $(dir $@)

	@echo "{Makefile} Building $@"
	$(CC) -c $< -o $@ $(C_FLAGS)

# Can be called to destroy all build files
# A clean build might rarely result in a bug fix
clean:
	rm -rf $(OBJ_DIR) # OBJ_DIR != / O_O
	rm $(EXE_NAME)
```

## Simple 2D Math Abstractions

Libraries such as `glm`, and the C equivalent `cglm`, take advantage
of special CPU features (SIMD instructions, single-instruction
multiple-data) to speed up vector operations. This introduces the
burden of writing assembly and platform-specific code. We won't be
performing many vector operations anyways, so we can just implement a
trivial struct as of now.

```c
#ifndef _NG_MATH_H
#define _NG_MATH_H

typedef struct
{
    float x, y;
} ng_vec2_t;

// Much faster to just copy and return by value,
// no need to introduce indirection (pointers) like previously.
// Let's hope that our compiler will store this struct on a set of registers
ng_vec2_t ng_vec2_add(ng_vec2_t first, ng_vec2_t second);
ng_vec2_t ng_vec2_subtract(ng_vec2_t first, ng_vec2_t second);
ng_vec2_t ng_vec2_multiply_by_scalar(ng_vec2_t vec, float scalar);

float ng_vec2_get_magnitude(ng_vec2_t vector);
ng_vec2_t ng_vec2_normalize(ng_vec2_t src);
```

```c
#include <math.h>

// Much faster than pow(, 2)!
#define SQUARED(x) (x * x)

ng_vec2_t ng_vec2_add(ng_vec2_t first, ng_vec2_t second)
{
    return (ng_vec2_t) { first.x + second.x, first.y + second.y };
}

// TODO: Implement the boring remainder yourself

float ng_vec2_get_magnitude(ng_vec2_t vector)
{
    return sqrt(SQUARED(vector.x) + SQUARED(vector.y));
}

ng_vec2_t ng_vec2_normalize(ng_vec2_t src)
{
    // Normalized vectors are super important! They represent plain directions
    return ng_vec2_multiply_by_scalar(src, 1.0f / ng_vec2_get_magnitude(src));
}
```
