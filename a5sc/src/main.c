#include <stdio.h>
#include "game.h"
#include "gamestate.h"

// Screen display size
#define AL_SCREEN_WIDTH     640
#define AL_SCREEN_HEIGHT    480

// Window title
#define AL_WINDOW_TITLE     "Allegro application"

// Frame-rate (FPS)
#define AL_REFRESH_RATE     40

// Start in full-screen?
static int AL_USE_FULLSCREEN = 0;

// Use audio module?
static int AL_ENABLE_AUDIO = 0;

//---------------------------------------------------
// The main procedure
//---------------------------------------------------
int main(int argc, char* argv[])
{
    // Default configuration that the game will make use of
    struct Game_Config config =
    {
        AL_SCREEN_WIDTH,
        AL_SCREEN_HEIGHT,
        AL_USE_FULLSCREEN,
        AL_REFRESH_RATE,
        AL_ENABLE_AUDIO
    };

    if (game_init(&config, AL_WINDOW_TITLE))
    {
        // This is the starting state
        // The second argument is a pointer that will be passed to the init() function
        // of the state (a void*)
        change_state(GAME_STATE, NULL);

        // Run the engine
        game_run();
    }

    return 0;
}
