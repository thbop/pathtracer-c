#ifndef VEC3_H
#define VEC3_H

#include "SDL3/SDL.h"

typedef struct {
    float x, y, z;
} vec3;

#define VEC3_ZERO ( (vec3){ 0.0f, 0.0f, 0.0f } )

vec3 vec3_add( vec3 *a, vec3 *b ) {
    return (vec3){
        a->x + b->x,
        a->y + b->y,
        a->z + b->z,
    };
}

vec3 vec3_add_value( vec3 *a, float value ) {
    return (vec3){
        a->x + value,
        a->y + value,
        a->z + value,
    };
}

vec3 vec3_sub( vec3 *a, vec3 *b ) {
    return (vec3){
        a->x - b->x,
        a->y - b->y,
        a->z - b->z,
    };
}

vec3 vec3_sub_value( vec3 *a, float value ) {
    return (vec3){
        a->x - value,
        a->y - value,
        a->z - value,
    };
}
vec3 vec3_mul( vec3 *a, vec3 *b ) {
    return (vec3){
        a->x * b->x,
        a->y * b->y,
        a->z * b->z,
    };
}

vec3 vec3_mul_value( vec3 *a, float value ) {
    return (vec3){
        a->x * value,
        a->y * value,
        a->z * value,
    };
}
vec3 vec3_div( vec3 *a, vec3 *b ) {
    return (vec3){
        a->x / b->x,
        a->y / b->y,
        a->z / b->z,
    };
}

vec3 vec3_div_value( vec3 *a, float value ) {
    float inv_value = 1.0f / value;
    return (vec3){
        a->x * inv_value,
        a->y * inv_value,
        a->z * inv_value,
    };
}

float vec3_dot( vec3 *a, vec3 *b ) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float vec3_length( vec3 *a ) {
    return SDL_sqrt( vec3_dot( a, a ) );
}

// Adapted from: https://en.wikipedia.org/wiki/Fast_inverse_square_root
float Q_rsqrt( float number ) {
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );

	return y;
}

float vec3_rlength( vec3 *a ) {
    return Q_rsqrt( vec3_dot( a, a ) );
}

vec3 vec3_normalize( vec3 *a ) {
    return vec3_mul_value( a, vec3_rlength( a ) );
}

// a points to b
vec3 vec3_point_to( vec3 *start, vec3 *target ) {
    vec3 difference = vec3_sub( target, start );
    return vec3_normalize( &difference );
}

#endif