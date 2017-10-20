#include <allegro.h>
#include "engine.h"
#include "state_game.h"

int main(int argc, char **argv)
{
  struct Engine_Conf conf =
    {
      argc, argv,             // Argument list
      "Allegro application",  // Window title
      640, 480,               // Resolution
      40,                     // Frame-rate (or FPS)
      8,                      // Color depth (8, 15, 16, 24, 32)
      FALSE                   // Full-screen?
    };

  if (engine_init(&conf))
    {
      // Run with a starting state
      engine_run(STATE_GAME);
    }

  return 0;
}
END_OF_MAIN()
