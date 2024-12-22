#include "audio.h"
#include "common.h"

Mix_Chunk* ng_audio_load(const char *file)
{
#ifndef NO_AUDIO
    Mix_Chunk *audio = Mix_LoadWAV(file);

    // Making sure that the audio file was successfully loaded
    if (!audio)
        ng_die("Something went wrong, couldn't load audio file %s!", file);

    return audio;
#endif
}

void ng_audio_play(Mix_Chunk *audio)
{
#ifndef NO_AUDIO
    Mix_PlayChannel(-1, audio, 0);
#endif
}
