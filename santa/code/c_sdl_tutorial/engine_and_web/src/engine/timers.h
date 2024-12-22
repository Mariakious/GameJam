#ifndef _NG_TIMER_H
#define _NG_TIMER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * These timers amount for real-world time, not for game time. That's why they
 * do not require delta time as input. You might want to change that depending
 * on your use case
 */
typedef struct
{
    bool is_active;
    uint32_t starting_time;
} ng_timer_t;

void ng_timer_start(ng_timer_t *timer);

uint32_t ng_timer_get_elapsed(ng_timer_t *timer);
// Just restarts the timer and returns the elapsed time in milliseconds
uint32_t ng_timer_restart(ng_timer_t *timer);

typedef struct
{
    uint32_t starting_time;
    
    // How long should it wait between each "event fire"
    uint32_t duration;
} ng_interval_t;

void ng_interval_create(ng_interval_t *interval, uint32_t duration);
bool ng_interval_is_ready(ng_interval_t *interval);

#endif
