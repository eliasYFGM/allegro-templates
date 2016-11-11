#include "engine.h"
#include "game_state.h"

int main(int argc, char **argv)
{
  // Default configuration that the game will make use of
  struct Game_Config config =
  {
    // Argument list
    argc, argv,
    // Window title
    "Allegro application",
    // Display resolution
    640, 480,
    // Framerate (FPS)
    40,
    // Full-screen?
    FALSE,
    // Enable audio module?
    FALSE,
    // Additional back-buffer? (black bars on full-screen)
    FALSE
  };

  if (game_init(&config))
  {
    // Run the game, with a starting state
    game_run(GAME_STATE, NULL);
  }

  return 0;
}
