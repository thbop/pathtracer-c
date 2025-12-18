#include "SDL3/SDL.h"

#define WIDTH  640
#define HEIGHT 360

#define SDL_ASSERT( expr ) \
    if ( !( expr ) ) \
        SDL_Log( "ERROR: %s\n", SDL_GetError() )


static struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} state;


int main() {
    // Setup
    SDL_ASSERT( SDL_Init( SDL_INIT_VIDEO ) );
    SDL_ASSERT( SDL_CreateWindowAndRenderer( "Pathtacer", WIDTH, HEIGHT, 0, &state.window, &state.renderer ) );


    // Mainloop
    bool running = true;
    while ( running ) {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_EVENT_QUIT ) {
                running = false;
            }
        }
    }

    // Cleanup
    SDL_DestroyWindow( state.window );
    SDL_Quit();
    return 0;
}