#include "timers.h"
#include <SDL2/SDL.h>

void ng_timer_start(ng_timer_t *timer)
{
    // Remember: SDL_GetTicks() returns milliseconds since SDL initialization
    timer->starting_time = SDL_GetTicks();
    timer->is_active = true;
}

uint32_t ng_timer_get_elapsed(ng_timer_t *timer)
{
    return SDL_GetTicks() - timer->starting_time;
}

uint32_t ng_timer_restart(ng_timer_t *timer)
{
    uint32_t now = SDL_GetTicks();
    uint32_t elapsed = now - timer->starting_time;

    // Restart the timer by making it count time since now
    timer->starting_time = SDL_GetTicks();

    return elapsed;
}

void ng_interval_create(ng_interval_t *interval, uint32_t duration)
{
    interval->duration = duration;
    interval->starting_time = SDL_GetTicks();
}

// Returns true whenever the interval has completed,
// then automatically restarts it back to the beginning
// It's like a timer, but it repeats
bool ng_interval_is_ready(ng_interval_t *interval)
{
    if (SDL_GetTicks() - interval->starting_time > interval->duration)
    {
        // If the interval has been reached, restart the timer and return true
        interval->starting_time = SDL_GetTicks();

        return true;
    }

    return false;
}
