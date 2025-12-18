#include "SDL3/SDL.h"

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
    float
        dx = 1.0f / (float)surf->w,
        dy = 1.0f / (float)surf->h,
        x  = 0.0f,
        y  = 0.0f;
    
    for ( int j = 0; j < surf->h; j++ ) {
        for ( int i = 0; i < surf->w; i++ ) {
            x += dx;
            SDL_WriteSurfacePixelFloat( surf, i, j, x, y, SDL_fabsf( SDL_sinf( (x + y)*2 ) ), 1.0f );
        }
        x = 0.0f;
        y += dy;
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
            if ( event.type == SDL_EVENT_QUIT ) {
                running = false;
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