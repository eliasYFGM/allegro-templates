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
  };

  if (game_init(&config))
  {
    // Run the game with a default "starting state"
    game_run(GAME_STATE, NULL);
  }

  return 0;
}
