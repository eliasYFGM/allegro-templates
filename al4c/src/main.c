#include "core.h"
#include "game_state.h"

int main(int argc, char** argv)
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
    FALSE,
    // Want audio module?
    FALSE,
    // Default argument list
    { argc, argv }
  };

  if (game_init(&config))
  {
    // Run the game with a default "starting state"
    game_run(GAME_STATE);
  }

  return 0;
}
END_OF_MAIN()
