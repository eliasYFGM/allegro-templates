#include <allegro.h>
#include "engine.h"
#include "state_game.h"

int main(int argc, char **argv)
{
  struct Engine_Conf conf =
    {
      // Argument list
      argc, argv,
      // Window title
      "Allegro application",
      // Resolution
      320, 200,
      // Frame-rate (or FPS)
      40,
      // Color depth (8, 15, 16, 24, 32)
      8
    };

  if (engine_init(&conf))
    {
      // Run with a starting state
      engine_run(STATE_GAME);
    }

  return 0;
}
END_OF_MAIN()
