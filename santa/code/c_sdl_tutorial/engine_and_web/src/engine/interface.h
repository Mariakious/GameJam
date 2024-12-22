#ifndef _NG_INTERFACE_H
#define _NG_INTERFACE_H

#include <stdbool.h>
#include "sprite.h"

// Labels will inherit from sprite too
typedef struct
{
    ng_sprite_t sprite;

    TTF_Font *font;
    unsigned int wrap_length;
} ng_label_t;

// NOTE: Leave wrap_length to 0 for default rendering in a single line
// The wrap width should be provided in pixels
void ng_label_create(ng_label_t *label, TTF_Font *font, unsigned int wrap_length);

void ng_label_set_content(ng_label_t *label, SDL_Renderer *renderer, const char *content);
void ng_label_destroy(ng_label_t *label);

#endif
