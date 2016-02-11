#include <iostream>
#include "gameengine.h"
#include "gamestate.h"

// Window title
#define AL_WINDOW_TITLE     "Allegro application"

// Display resolution
#define AL_SCREEN_WIDTH     640
#define AL_SCREEN_HEIGHT    480

// Framerate (FPS)
#define AL_REFRESH_RATE     40

// Color depth (8, 15, 16, 24, 32)
#define AL_COLOR_DEPTH      8

// Whether to use fullscreen
static bool AL_USE_FULLSCREEN = false;

//---------------------------------------------------
// The main procedure
//---------------------------------------------------
int main(int argc, char* argv[])
{
    Game_Engine game;

    // Initialization
    bool result = game.init(
        AL_WINDOW_TITLE,
        AL_USE_FULLSCREEN,
        AL_SCREEN_WIDTH,
        AL_SCREEN_HEIGHT,
        AL_REFRESH_RATE,
        AL_COLOR_DEPTH
    );

    if (!result)
    {
        std::cout << "Something went wrong during initialization" << std::endl;
        return 1;
    }

    // This is the starting state
    game.change_state(new Game_State(&game));

    // Run the game engine
    game.run();

    // Shutdown
    game.end();

    return 0;
}
END_OF_MAIN()
