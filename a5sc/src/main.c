#include <stdio.h>
#include "game.h"
#include "gamestate.h"

int main(int argc, char* argv[])
{
    // Default configuration that the game will make use of
    struct Game_Config config =
    {
        // Window title
        "Allegro application",
        // Display resolution
        640, 480,
        // Framerate (FPS)
        40,
        // Want full-screen?
        0,
        // Want audio module?
        0
    };

    if (game_init(config))
    {
        // Starting state
        // Second argument is a pointer that will be passed to the init() function
        // of the state (a void*)
        change_state(GAME_STATE, NULL);

        // Run the game until it's done
        game_run();
    }

    return 0;
}
