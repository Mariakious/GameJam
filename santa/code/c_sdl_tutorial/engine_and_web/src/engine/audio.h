#ifndef _NG_AUDIO_H
#define _NG_AUDIO_H

#include <SDL2/SDL_mixer.h>

Mix_Chunk* ng_audio_load(const char *file);
void ng_audio_play(Mix_Chunk *audio);

#endif
