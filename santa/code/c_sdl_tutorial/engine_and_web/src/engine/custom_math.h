#ifndef _NG_CUSTOM_MATH_H
#define _NG_CUSTOM_MATH_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct
{
    float x, y;
} ng_vec2;

float ng_vector_get_magnitude(ng_vec2 *source);
void ng_vector_normalize(ng_vec2 *result, ng_vec2 *source);
void ng_vector_multiply_by(ng_vec2 *result, ng_vec2 *source, float scalar);

void ng_vectors_add(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second);
void ng_vectors_substract(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second);
void ng_vectors_multiply(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second);
void ng_vectors_divide(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second);

// Other math-related functions
bool ng_is_point_inside(SDL_Rect *rect, int x, int y);
int ng_get_distance(int x, int y, int other_x, int other_y);

#endif
