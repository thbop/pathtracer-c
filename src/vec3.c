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

#include "vec3.h"


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

vec3 vec3_cross( vec3 *a, vec3 *b ) {
    return (vec3){
        a->x*b->x + a->x*b->y + a->x*b->z,
        a->y*b->x + a->y*b->y + a->y*b->z,
        a->z*b->x + a->z*b->y + a->z*b->z,
    };
}

float vec3_dot( vec3 *a, vec3 *b ) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

float vec3_square_length( vec3 *a ) {
    return vec3_dot( a, a );
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
        (float)rand() - ( RAND_MAX >> 1 ),
        (float)rand() - ( RAND_MAX >> 1 ),
        (float)rand() - ( RAND_MAX >> 1 ),
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
        if ( vec3_dot( normal, &p ) > 0.0f )
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


vec3 vec3_reflect( vec3 *incoming, vec3 *normal ) {
    float a = vec3_dot( incoming, normal ) * 2;
    vec3 b = vec3_mul_value( normal, a );

    return vec3_sub( incoming, &b );
}