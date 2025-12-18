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
        x - camera->width  * 0.5f,
        y - camera->height * 0.5f,
        camera->focal_length
    };

    ray->origin = camera->origin;
    ray->direction = vec3_point_to( &ray->origin, &pixel_pos );
}

#endif