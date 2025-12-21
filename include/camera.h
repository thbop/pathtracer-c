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

#ifndef CAMERA_H
#define CAMERA_H

#include "SDL3/SDL.h"
#include "vec3.h"
#include "ray.h"


typedef struct {
    vec3 origin;
    float fov, width, height;

    float focal_length; // Can be set with FOV
} camera_t;

void camera_compute_focal_length( camera_t *camera ) {
    float fov_radians = camera->fov * SDL_PI_F * 0.0055555555555555555f;
    camera->focal_length = camera->width / ( SDL_tanf( fov_radians * 0.5f ) * 2.0f );
}

void camera_generate_ray( ray_t *ray, camera_t *camera, float x, float y ) {
    vec3 pixel_pos = {
        x - camera->width * 0.5f,
        - y + camera->height * 0.5f,
        camera->focal_length
    };

    ray->origin = camera->origin;
    ray->direction = vec3_point_to( &ray->origin, &pixel_pos );
}

#endif