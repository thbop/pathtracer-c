/*
* Copyright © 2025 Thbop
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the “Software”), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef VEC3_H
#define VEC3_H

#include "SDL3/SDL.h"
#include "stdlib.h"

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

vec3 vec3_random() {
    vec3 p = {
        rand() * 12982.02345f,
        rand() * 23940.21398f,
        rand() * 62915.29356f,
    };

    return p;
}

vec3 vec3_random_unit() {
    vec3 p = vec3_random();
    return vec3_normalize( &p );
}

vec3 vec3_random_unit_hemisphere( vec3 *normal ) {
    while ( true ) {
        vec3 p = vec3_random();
        if ( vec3_dot( normal, &p ) )
            return vec3_normalize( &p );
    }
}

vec3 vec3_lerp( vec3 *a, vec3 *b, float t ) {
    t = SDL_clamp( t, 0.0f, 1.0f );
    vec3
        a_weight = vec3_mul_value( a, ( 1.0f - t ) ),
        b_weight = vec3_mul_value( b, t );
    return vec3_add( &a_weight, &b_weight );
}

#endif