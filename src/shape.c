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

#include "shape.h"

float shape_sphere_hit( void *shape, ray_path_t *ray_path ) {
    shape_t *sh = shape;

    shape_sphere_data_t *sphere_data = sh->data;

    vec3 diff = vec3_sub( &ray_path->ray.origin, &sh->position );
    float
        b = 2.0f * vec3_dot( &diff, &ray_path->ray.direction ),
        c = vec3_square_length( &diff ) - sphere_data->radius*sphere_data->radius,
        discriminant = b*b - 4.0f*c;
    
    if ( discriminant > 0 ) { // If hit (positive or negative)
        float t = ( -b - SDL_sqrtf( discriminant ) ) * 0.5f;
        if ( t > 0.0001f ) { // If positive
            vec3 hit_position = ray_at( &ray_path->ray, t );
            ray_path->hit_normal = vec3_point_to( &sh->position, &hit_position );
            return t;
        }
    }
    return -1.0f;
}

float shape_triangle_hit( void *shape, ray_path_t *ray_path ) {
    shape_t *sh = shape;
    shape_triangle_data_t *tri_dat = sh->data;

    // Adapted from Real-Time Rendering 4th Edition by Akenine-Möller on page 965
    vec3
        e0 = vec3_sub( tri_dat->p1, tri_dat->p0 ),
        e1 = vec3_sub( tri_dat->p2, tri_dat->p0 ),
        q  = vec3_cross( &ray_path->ray.direction, &e1 );
    float a = vec3_dot( &e0, &q );

    // If not near zero
    if ( a > -1e-10f && a < 1e-10f ) return -1.0f;

    float f = 1.0f / a;
    vec3 s = vec3_sub( &ray_path->ray.origin, tri_dat->p0 );
    float u = f * vec3_dot( &s, &q );

    // If outside triangle
    if ( u < 0.0f ) return -1.0f;

    vec3 r = vec3_cross( &s, &e0 );
    float v = f * vec3_dot( &ray_path->ray.direction, &r );

    // Also if outside triangle
    if ( v < 0.0f || u + v > 1.0f ) return -1.0f;

    float t = f * vec3_dot( &e1, &r );

    // Cull if intersection occurs behind the ray
    if ( t < 0.0f ) return -1.0f;

    // NOTE: u and v can be used for texture coordinates

    ray_path->hit_normal = *tri_dat->normal;

    return t;
}