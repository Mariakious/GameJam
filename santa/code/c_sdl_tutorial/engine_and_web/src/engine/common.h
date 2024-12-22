#ifndef _NG_COMMON_H
#define _NG_COMMON_H

#include <stdbool.h>

// Some simple macros
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

// Just prints out the messages and kills the program
void ng_die(const char *format, ...);

int ng_random_int_in_range(int start, int end);
bool ng_random_bool(void);

#endif
