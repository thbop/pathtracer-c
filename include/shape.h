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

#ifndef SHAPE_H
#define SHAPE_H

#include "vec3.h"
#include "ray.h"
#include "materials.h"


typedef struct {
    vec3 position;
    void *data;
    material_t material;

    float (*hit)( void*, ray_path_t* );
} shape_t;

typedef struct {
    float radius;
} shape_sphere_data_t;

typedef struct {
    vec3 *p0, *p1, *p2, *normal;
} shape_triangle_data_t;

// Hit functions
//     Will return a positive t upon hitting the object
//     Will set the hit_normal
float shape_sphere_hit( void *shape, ray_path_t *ray_path );

float shape_triangle_hit( void *shape, ray_path_t *ray_path );

#endif