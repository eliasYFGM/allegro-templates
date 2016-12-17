#include <allegro.h>
#include "engine.h"
#include "game_state.h"

int main(int argc, char **argv)
{
  struct Game_Config config =
  {
    // Argument list
    argc, argv,
    // Window title
    "Allegro application",
    // Game resolution
    320, 200,
    // Scaling factor (<= 0 for auto)
    0,
    // Frame-rate (or FPS)
    40,
    // Color depth (8, 15, 16, 24, 32)
    8,
    // Enable audio module?
    FALSE
  };

  if (game_init(&config))
  {
    // Run the game, with a starting state
    game_run(Game_State());
  }

  return 0;
}
END_OF_MAIN()
