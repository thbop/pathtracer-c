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

#include "stdio.h"
#include "float.h"

#include "SDL3/SDL.h"

#include "chelp/list_t.h"

#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "materials.h"
#include "shape.h"

#if __INTELLISENSE__
    #define constexpr // Intellisense doesn't like the c23 standard
#endif

#define WINDOW_WIDTH      1280 // Resolution
#define WINDOW_HEIGHT     720
#define RENDER_WIDTH      1280 // Upscaled to window size
#define RENDER_HEIGHT     720
#define PIXEL_SUBDIVISION 16   // Antialiasing/samples (1 = none)
#define RAY_MAX_BOUNCES   10


#define SDL_ASSERT( expr ) \
    if ( !( expr ) ) \
        SDL_Log( "ERROR: %s\n", SDL_GetError() )


static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *screen;

    list_t shapes;

    SDL_AtomicInt render_progress;
} state;


vec3 sky_color( ray_path_t *ray_path ) {
    float t = ( ray_path->ray.direction.y + 1.0f ) * 0.5f;
    vec3
        white = (vec3){ 0.7f, 0.7f, 0.7f },
        blue  = (vec3){ 0.65f, 0.65f, 0.70f };
    
    return vec3_lerp( &blue, &white, t );
}

vec3 ray_hit( ray_path_t *ray_path ) {
    if ( ray_path->bounces == 0 )
        return VEC3_ZERO;

    ray_path->bounces--;

    float closest_t = __FLT32_MAX__;
    shape_t *closest_shape;
    list_foreach( state.shapes, it ) {
        shape_t *shape = it->value;
        float t = shape->hit( shape, ray_path );

        if ( t >= 0.0f && t < closest_t ) {
            closest_t = t;
            closest_shape = shape;
        }
    }

    if ( closest_t < __FLT32_MAX__ ) {
        ray_path->ray.origin = ray_at( &ray_path->ray, closest_t );
        vec3
            this_color = closest_shape->material.processor( &closest_shape->material, ray_path ),
            next_color = ray_hit( ray_path );
        return vec3_mul( &this_color, &next_color );
    }
    // Otherwise
    return sky_color( ray_path );
}

static int SDLCALL render( void *surface ) {
    SDL_Surface *surf = surface;
    camera_t camera = {
        .origin = VEC3_ZERO,
        .width  = RENDER_WIDTH,
        .height = RENDER_HEIGHT,
        .fov    = 90.0f,
    };
    camera_compute_focal_length( &camera );

    float subpixel_size = 1.0f / ( PIXEL_SUBDIVISION*PIXEL_SUBDIVISION );
    
    for ( int j = 0; j < surf->h; j++ ) {
        for ( int i = 0; i < surf->w; i++ ) {
            vec3 color = VEC3_ZERO;
            for ( int s_j = 0; s_j < PIXEL_SUBDIVISION; s_j++ ) {
                for ( int s_i = 0; s_i < PIXEL_SUBDIVISION; s_i++ ) {
                    ray_path_t ray_path = {
                        .bounces = RAY_MAX_BOUNCES,
                    };
                    camera_generate_ray( &ray_path.ray, &camera, i + s_i * subpixel_size, j + s_j * subpixel_size );
                    vec3 raw_ray_color = ray_hit( &ray_path );
                    color = vec3_add( &color, &raw_ray_color );
                }
            }
            color = vec3_mul_value( &color, subpixel_size );
            SDL_WriteSurfacePixelFloat( surf, i, j, color.x, color.y, color.z, 1.0f );
        }

        // Update progress
        SDL_SetAtomicInt( &state.render_progress, ( j * 25 ) / surf->h );
    }
    
    SDL_SetAtomicInt( &state.render_progress, 25 );

    return 0;
}

void render_call() {
    SDL_ASSERT( SDL_LockTextureToSurface( state.texture, NULL, &state.screen ) );
    SDL_SetAtomicInt( &state.render_progress, 0 );
    SDL_Thread *thread = SDL_CreateThread( render, "render", state.screen );
    SDL_DetachThread( thread );
}

void render_finish() {
    SDL_UnlockTexture( state.texture );
}

void display_progress() {
    int progress = SDL_GetAtomicInt( &state.render_progress );
    if ( progress < 25 ) {
        printf( "\rProgress: <" );
        for ( int i = 0; i < 25; i++ ) {
            if ( i <= progress )
                putchar( '=' );
            else
                putchar( ' ' );
        }
        putchar( '>' );
    }
}

int main() {
    // Setup
    SDL_ASSERT( SDL_Init( SDL_INIT_VIDEO ) );
    SDL_ASSERT( SDL_CreateWindowAndRenderer(
        "Pathtracer",
        WINDOW_WIDTH, WINDOW_HEIGHT, 0,
        &state.window,
        &state.renderer
    ) );
    SDL_ASSERT( state.texture = SDL_CreateTexture(
        state.renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        RENDER_WIDTH, RENDER_HEIGHT
    ) );
    SDL_SetTextureScaleMode( state.texture, SDL_SCALEMODE_NEAREST );

    state.shapes = new_list();

    // TEMP
    shape_sphere_data_t sphere_data;
    shape_t sphere;

    sphere_data.radius = 4.0f;
    sphere = (shape_t){
        .position = { 0.0f, 0.0f, 20.0f },
        .data     = qalloc( sphere_data ),
        .material = {
            .color     = { 0.6f, 0.6f, 0.8f },
            .processor = material_diffuse,
        },
        .hit = shape_sphere_hit,
    };
    list_append( state.shapes, sphere );

    sphere_data.radius = 1000.0f;
    sphere = (shape_t){
        .position = { 0.0f, -1004.0f, 20.0f },
        .data     = qalloc( sphere_data ),
        .material = {
            .color     = { 0.43f, 0.43f, 0.4f },
            .processor = material_diffuse,
        },
        .hit = shape_sphere_hit,
    };
    list_append( state.shapes, sphere );

    render_call();

    // Mainloop
    bool running = true;
    while ( running ) {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            switch ( event.type ) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if ( SDL_GetAtomicInt( &state.render_progress ) >= 25 )
                        render_call();
            }
        }

        if ( SDL_GetAtomicInt( &state.render_progress ) >= 25 )
            render_finish();

        // Clear screen
        SDL_SetRenderDrawColor( state.renderer, 0, 0, 0, 255 );
        SDL_RenderClear( state.renderer );

        // Present changes on the screen
        SDL_RenderTexture( state.renderer, state.texture, NULL, NULL );
        SDL_RenderPresent( state.renderer );

        display_progress();
    }

    // Cleanup
    list_foreach( state.shapes, it ) {
        shape_t* shape = it->value;
        free( shape->data );
    }
    list_free( state.shapes );

    SDL_DestroyRenderer( state.renderer );
    SDL_DestroyWindow( state.window );
    SDL_Quit();
    return 0;
}