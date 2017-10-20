#include "engine.h"
#include "state_game.h"

int main(int argc, char **argv)
{
  struct Engine_Conf conf =
    {
      argc, argv,             // Argument list
      "Allegro application",  // Window title
      640, 480,               // Display resolution
      40,                     // Framerate (FPS)
      FALSE,                  // Full-screen?
      FALSE                   // Use back-buffer?
    };

  if (engine_init(&conf))
    {
      // Run with a starting state
      engine_run(STATE_GAME);
    }

  return 0;
}
