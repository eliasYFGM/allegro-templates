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
    320, 200,
    // Scaling factor (<= 0 for auto)
    0,
    // Framerate (FPS)
    40,
    // Want audio module?
    FALSE
  };

  if (game_init(&config))
  {
    // Starting state
    // Second argument is a value that can be passed to the init() function
    // of the state (long)
    change_state(GAME_STATE, 0);

    // Run the game until it's done
    game_run();
  }

  return 0;
}