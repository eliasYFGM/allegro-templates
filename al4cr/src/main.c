#include "core.h"
#include "game_state.h"

int main(int argc, char* argv[])
{
  struct Game_Config config =
  {
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
    // Want audio module?
    FALSE
  };

  if (game_init(&config))
  {
    // Starting state
    // Second argument is a value that can be passed to the init() function
    // of the state (long)
    change_state(GAME_STATE, 0);

    // Run the game
    game_run();
  }

  return 0;
}
END_OF_MAIN()