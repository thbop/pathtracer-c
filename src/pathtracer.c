#include "SDL3/SDL.h"

#include "vec3.h"
#include "ray.h"
#include "camera.h"

#define RENDER_WIDTH  320
#define RENDER_HEIGHT 180
#define WINDOW_WIDTH  RENDER_WIDTH * 4
#define WINDOW_HEIGHT RENDER_HEIGHT * 4

#define SDL_ASSERT( expr ) \
    if ( !( expr ) ) \
        SDL_Log( "ERROR: %s\n", SDL_GetError() )


static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *screen;

    SDL_AtomicInt render_progress;
} state;

static int SDLCALL render( void *surface ) {
    SDL_Surface *surf = surface;
    camera_t camera = {
        .origin = VEC3_ZERO,
        .width  = RENDER_WIDTH,
        .height = RENDER_HEIGHT,
        .fov    = 120.0f,
    };
    camera_compute_focal_length( &camera );
    
    ray_t ray;
    for ( int j = 0; j < surf->h; j++ ) {
        for ( int i = 0; i < surf->w; i++ ) {
            camera_generate_ray( &ray, &camera, i, j );
            SDL_WriteSurfacePixelFloat( surf, i, j, ray.direction.x, ray.direction.y, ray.direction.z, 1.0f );
        }
    }
    
    SDL_SetAtomicInt( &state.render_progress, 100 );

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

int main() {
    // Setup
    SDL_ASSERT( SDL_Init( SDL_INIT_VIDEO ) );
    SDL_ASSERT( SDL_CreateWindowAndRenderer(
        "Pathtacer",
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
                    if ( SDL_GetAtomicInt( &state.render_progress ) == 100 )
                        render_call();
            }
        }

        if ( SDL_GetAtomicInt( &state.render_progress ) == 100 )
            render_finish();

        // Clear screen
        SDL_SetRenderDrawColor( state.renderer, 0, 0, 0, 255 );
        SDL_RenderClear( state.renderer );

        // Present changes on the screen
        SDL_RenderTexture( state.renderer, state.texture, NULL, NULL );
        SDL_RenderPresent( state.renderer );
    }

    // Cleanup
    SDL_DestroyRenderer( state.renderer );
    SDL_DestroyWindow( state.window );
    SDL_Quit();
    return 0;
}