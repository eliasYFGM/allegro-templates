#include <iostream>
#include "gameengine.h"
#include "gamestate.h"

// Screen display size
#define AL_SCREEN_WIDTH     640
#define AL_SCREEN_HEIGHT    480

// Window title
#define AL_WINDOW_TITLE     "Allegro application"

// Framerate (FPS)
#define AL_REFRESH_RATE     40

// Start in fullscreen?
static bool AL_USE_FULLSCREEN = false;

// Use audio module?
static bool AL_ENABLE_AUDIO = false;

//---------------------------------------------------
// The main procedure
//---------------------------------------------------
int main(int argc, char* argv[])
{
    Game_Engine game;

    // Initialization
    bool result = game.init(
        AL_SCREEN_WIDTH,
        AL_SCREEN_HEIGHT,
        AL_USE_FULLSCREEN,
        AL_REFRESH_RATE,
        AL_ENABLE_AUDIO,
        AL_WINDOW_TITLE
    );

    if (!result)
    {
        std::cout << "There was an error while initializing the Game Engine" << std::endl;
        return -1;
    }

    // This is the starting state
    game.change_state(new Game_State(&game));

    // Run the engine
    game.run();

    // Shutdown
    game.end();

    return 0;
}
