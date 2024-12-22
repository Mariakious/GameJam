#include "custom_math.h"
#include <math.h>

#define SQUARE(x) (x * x)

float ng_vector_get_magnitude(ng_vec2 *source)
{
    return sqrt(SQUARE(source->x) + SQUARE(source->y));
}

void ng_vector_multiply_by(ng_vec2 *result, ng_vec2 *source, float scalar)
{
    result->x = source->x * scalar;
    result->y = source->y * scalar;
}

void ng_vector_normalize(ng_vec2 *result, ng_vec2 *source)
{
    float magnitude = ng_vector_get_magnitude(source);

    result->x = source->x / magnitude;
    result->y = source->y / magnitude;
}

// Since operator overloading is not available, I have to do it this way
void ng_vectors_add(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second)
{
    result->x = first->x + second->x;
    result->y = first->y + second->y;
}

void ng_vectors_substract(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second)
{
    result->x = first->x - second->x;
    result->y = first->y - second->y;
}

void ng_vectors_multiply(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second)
{
    result->x = first->x * second->x;
    result->y = first->y * second->y;
}

void ng_vectors_divide(ng_vec2 *result, ng_vec2 *first, ng_vec2 *second)
{
    result->x = first->x / second->x;
    result->y = first->y / second->y;
}

bool ng_is_point_inside(SDL_Rect *rect, int x, int y)
{
    // This function only works for rectangles
    return x > rect->x && x < rect->x + rect->w &&
           y > rect->y && y < rect->y + rect->h;
}

int ng_get_distance(int x, int y, int other_x, int other_y)
{
    // Basically getting the magnitude of the distance vector
    return sqrt(SQUARE(other_x - x) + SQUARE(other_y - y));
}
