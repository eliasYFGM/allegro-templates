#include <stdio.h>
#include "core.h"
#include "game_state.h"

int main(int argc, char** argv)
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
    FALSE,
    // Want audio module?
    FALSE,
    // Use additional back-buffer? (black bars on full-screen)
    FALSE,
    // Argument list
    { argc, argv }
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
