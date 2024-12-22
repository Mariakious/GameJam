#include "sprite.h"
#include "common.h"

void ng_sprite_create(ng_sprite_t *sprite, SDL_Texture *texture)
{
    if (!texture)
        ng_die("failed to create sprite, an invalid texture was provided");
    
    sprite->texture = texture;
    sprite->src.x = sprite->src.y = 0;

    // Fetching the texture's dimensions and saving them in the sprite's source rect 
    SDL_QueryTexture(texture, NULL, NULL, &sprite->src.w, &sprite->src.h);

    sprite->transform.x = sprite->transform.y = 0;
    ng_sprite_set_scale(sprite, 1.0f);
}

void ng_sprite_set_scale(ng_sprite_t *sprite, float scale)
{
    // They can only appear as multiples of the base size,
    // because otherwise the sprite will appear stretched and ugly
    sprite->transform.w = sprite->src.w * scale;
    sprite->transform.h = sprite->src.h * scale;
}

void ng_sprite_render(ng_sprite_t *sprite, SDL_Renderer *renderer)
{
    SDL_RenderCopyF(renderer, sprite->texture, &sprite->src, &sprite->transform);
}

void ng_animated_create(ng_animated_sprite_t *anim, SDL_Texture *texture,
                        unsigned int total_frames)
{
    ng_sprite_create(&anim->sprite, texture);

    anim->total_frames = total_frames = total_frames;
    anim->frame = 0;

    // Adjust source size, we are only interested in a single frame
    anim->sprite.src.w /= total_frames;
    anim->sprite.transform.w = anim->sprite.src.w;
}

void ng_animated_set_frame(ng_animated_sprite_t *anim, int frame_index)
{
    anim->frame = frame_index;
    anim->sprite.src.x = frame_index * anim->sprite.src.w;
}
