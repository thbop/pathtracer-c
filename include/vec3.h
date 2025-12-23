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
#include "math.h"

typedef struct {
    float x, y, z;
} vec3;

#define VEC3_ZERO ( (vec3){ 0.0f, 0.0f, 0.0f } )

vec3 vec3_add( vec3 *a, vec3 *b );
vec3 vec3_add_value( vec3 *a, float value );
vec3 vec3_sub( vec3 *a, vec3 *b );
vec3 vec3_sub_value( vec3 *a, float value );
vec3 vec3_mul( vec3 *a, vec3 *b );
vec3 vec3_mul_value( vec3 *a, float value );
vec3 vec3_div( vec3 *a, vec3 *b );
vec3 vec3_div_value( vec3 *a, float value );

float vec3_dot( vec3 *a, vec3 *b );

float vec3_square_length( vec3 *a );
float vec3_length( vec3 *a );
float vec3_rlength( vec3 *a );

vec3 vec3_normalize( vec3 *a );

// start points to target
vec3 vec3_point_to( vec3 *start, vec3 *target );

vec3 vec3_random();
vec3 vec3_random_unit();
vec3 vec3_random_unit_hemisphere( vec3 *normal );

vec3 vec3_lerp( vec3 *a, vec3 *b, float t );

#endif