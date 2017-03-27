#include "engine.h"
#include "game_state.h"

int main(int argc, char **argv)
{
  // Default configuration that the engine will make use of
  struct Engine_Conf conf =
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
    // Additional back-buffer? (black bars on full-screen)
    FALSE
  };

  if (engine_init(&conf))
  {
    // Run with a starting state
    engine_run(GAME_STATE);
  }

  return 0;
}
