#include "interface.h"
#include <SDL2/SDL.h>

// Default font color
static SDL_Color white = {255, 255, 255, 255};

void ng_label_create(ng_label_t *label, TTF_Font *font, unsigned int wrap_length)
{
    // Initializing to NULL so that we don't free garbage
    label->sprite.texture = NULL;

    label->font = font;
    label->wrap_length = wrap_length;
}

void ng_label_set_content(ng_label_t *label, SDL_Renderer *renderer, const char *content)
{
    // Avoid the memory leak
    SDL_DestroyTexture(label->sprite.texture);
    
    SDL_Surface *surface = label->wrap_length > 0
        ? TTF_RenderText_Solid_Wrapped(label->font, content, white, label->wrap_length)
        : TTF_RenderText_Solid(label->font, content, white);

    ng_sprite_create(&label->sprite, SDL_CreateTextureFromSurface(renderer, surface));
    SDL_FreeSurface(surface);
}

void ng_label_destroy(ng_label_t *label)
{
    SDL_DestroyTexture(label->sprite.texture);
}
