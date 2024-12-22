#ifndef _NG_SPRITE_H
#define _NG_SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "custom_math.h"
#include "timers.h"

// Sprites are 2D entities that can be rendered using a simple texture
typedef struct
{
    SDL_Texture *texture;
    // Position and size inside texture atlas
    SDL_Rect src;

    // Position and target screen size
    SDL_FRect transform;
} ng_sprite_t;

void ng_sprite_create(ng_sprite_t *sprite, SDL_Texture *texture);
void ng_sprite_render(ng_sprite_t *sprite, SDL_Renderer *renderer);
void ng_sprite_set_scale(ng_sprite_t *sprite, float scale);

typedef struct
{
    // NOTE: Inheritance in C!
    // Casting this struct to ng_sprite_t* is allowed
    // because the first field inside it matches the type
    ng_sprite_t sprite;

    int frame;
    int total_frames;
} ng_animated_sprite_t;

// Some sprites will have a texture consisting of multiple frames inside a larger texture atlas
void ng_animated_create(ng_animated_sprite_t *anim, SDL_Texture *texture,
                        unsigned int total_frames);

void ng_animated_set_frame(ng_animated_sprite_t *anim, int frame_index);

#endif
