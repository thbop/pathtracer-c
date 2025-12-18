#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct {
    vec3 origin, direction;
} ray_t;


vec3 ray_at( ray_t *ray, float t ) {
    vec3 a = vec3_mul_value( &ray->direction, t );
    return vec3_add( &a, &ray->origin );
}

#endif