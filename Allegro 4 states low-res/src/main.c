#include <allegro.h>
#include "engine.h"
#include "game_state.h"

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
    // Scaling factor (<= 0 for auto)
    0,
    // Frame-rate (or FPS)
    40,
    // Color depth (8, 15, 16, 24, 32)
    8,
    // Enable audio module?
    FALSE
  };

  if (engine_init(&conf))
  {
    // Run with a starting state
    engine_run(GAME_STATE);
  }

  return 0;
}
END_OF_MAIN()
