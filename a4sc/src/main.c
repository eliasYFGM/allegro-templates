#include "game.h"
#include "gamestate.h"

// Window title
#define AL_WINDOW_TITLE     "Allegro application"

// Display resolution
#define AL_SCREEN_WIDTH     640
#define AL_SCREEN_HEIGHT    480

// Framerate (FPS)
#define AL_REFRESH_RATE     40

// Color depth
#define AL_COLOR_DEPTH      8

// Whether to use fullscreen
static int AL_USE_FULLSCREEN = 0;

// Whether to use audio module
static int AL_WANT_AUDIO = 0;

//---------------------------------------------------
// The main procedure
//---------------------------------------------------
int main(int argc, char* argv[])
{
    struct Game_Config config =
    {
        AL_SCREEN_WIDTH,
        AL_SCREEN_HEIGHT,
        AL_USE_FULLSCREEN,
        AL_REFRESH_RATE,
        AL_COLOR_DEPTH,
        AL_WANT_AUDIO
    };

    if (game_init(&config, AL_WINDOW_TITLE))
    {
        change_state(GAME_STATE, NULL);
        game_run();
    }

    return 0;
}
#ifdef _WIN32
END_OF_MAIN()
#endif
