#include <iostream>
#include "gameengine.h"
#include "gamestate.h"

int main(int argc, char* argv[])
{
    Game_Engine game;

    bool result = game.init(
        // Window title
        "Allegro application",
        // Game resolution
        640, 480,
        // Refresh rate
        40,
        // Color depth
        8,
        // Want full-screen?
        false,
        // Want audio module?
        false
    );

    if (result)
    {
        // Starting state
        game.change_state(new Game_State(&game));

        // Run the game engine
        game.run();
    }

    return 0;
}
END_OF_MAIN()
