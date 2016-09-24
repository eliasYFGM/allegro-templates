#include <stdio.h>
#include "core.h"
#include "game_state.h"

int main(int argc, char* argv[])
{
  // Default configuration that the game will make use of
  struct Game_Config config =
  {
    // Window title
    "Allegro application",
    // Internal resolution
    320, 200,
    // Scaling factor (<= 0 for auto)
    0,
    // Frame-rate (FPS)
    40,
    // Want audio module?
    FALSE
  };

  if (game_init(&config))
  {
    // The starting state
    change_state(GAME_STATE, NULL);

    // Run the game until it's done
    game_run();
  }

  return 0;
}
