#include <allegro5/allegro.h>
#include "engine.h"
#include "game_state.h"

int main(int argc, char** argv)
{
  // Default configuration that the game will make use of
  struct Game_Config config =
  {
    // Argument list
    argc, argv,
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
    // Run the game with a default "starting state"
    game_run(GAME_STATE, NULL);
  }

  return 0;
}
