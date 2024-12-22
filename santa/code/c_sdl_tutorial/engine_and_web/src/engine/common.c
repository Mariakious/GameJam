#include "common.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void ng_die(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // Just print out the supplied message with an informative prefix
    fprintf(stderr, "{awesome_sdl_engine fatal error} ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    
    va_end(args);
    exit(EXIT_FAILURE);
}

int ng_random_int_in_range(int start, int end)
{
    return start + rand() % (end - start);
}

bool ng_random_bool(void)
{
    return rand() % 2;
}
