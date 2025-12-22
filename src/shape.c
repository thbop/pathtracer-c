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

bool shape_sphere_hit( void *shape, ray_path_t *ray_path ) {
    shape_t *sh = shape;

    shape_sphere_data_t *sphere_data = sh->data;

    vec3 diff = vec3_sub( &ray_path->ray.origin, &sh->position );
    float
        b = 2.0f * vec3_dot( &diff, &ray_path->ray.direction ),
        c = vec3_square_length( &diff ) - sphere_data->radius*sphere_data->radius,
        discriminant = b*b - 4.0f*c;
    
    if ( discriminant > 0 ) { // If hit (positive or negative)
        float t = ( -b - SDL_sqrtf( discriminant ) ) / 2.0f;
        if ( t > 0.0f ) { // If positive
            ray_path->ray.origin = ray_at( &ray_path->ray, t );
            ray_path->hit_normal = vec3_point_to( &sh->position, &ray_path->ray.origin );
            return true;
        }
    }
    return false;
}