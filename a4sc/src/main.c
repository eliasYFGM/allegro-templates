#include "game.h"
#include "gamestate.h"

int main(int argc, char* argv[])
{
    struct Game_Config config =
    {
        // Window title
        "Allegro application",
        // Game resolution
        640, 480,
        // Refresh rate (or FPS)
        40,
        // Color depth (8, 15, 16, 24, 32)
        8,
        // Want full-screen?
        0,
        // Want audio module?
        0
    };

    if (game_init(&config))
    {
        // Starting state
        // Second argument is a pointer that will be passed to the init() function
        // of the state (a void* pointer)
        change_state(GAME_STATE, NULL);

        // Run the game
        game_run();
    }

    return 0;
}
#ifdef _WIN32
END_OF_MAIN()
#endif
