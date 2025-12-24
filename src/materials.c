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

#include "materials.h"


vec3 material_diffuse( void *material, ray_path_t *ray_path ) {
    material_t *mat = material;
    
    // ray_path->ray.direction = vec3_random_unit_hemisphere( &ray_path->hit_normal );

    // Source: https://raytracing.github.io/books/RayTracingInOneWeekend.html#diffusematerials/truelambertianreflection
    vec3 random_sphere = vec3_random_unit();
    ray_path->ray.direction = vec3_add( &ray_path->hit_normal, &random_sphere );

    if ( mat->color_setter == NULL )
        return mat->color;
    return mat->color_setter( ray_path );
}

vec3 material_metal( void *material, ray_path_t *ray_path ) {
    material_t *mat = material;

    ray_path->ray.direction = vec3_reflect( &ray_path->ray.direction, &ray_path->hit_normal );


    float roughness;
    if ( mat->roughness_setter != NULL )
        roughness = mat->roughness_setter( ray_path );
    else
        roughness = mat->roughness;

    if ( roughness > 0.0f ) {
        vec3 random_sphere = vec3_random_unit();
        random_sphere = vec3_mul_value( &random_sphere, roughness );
        ray_path->ray.direction = vec3_add( &ray_path->ray.direction, &random_sphere );
    }

    if ( mat->color_setter == NULL )
        return mat->color;
    return mat->color_setter( NULL );
}