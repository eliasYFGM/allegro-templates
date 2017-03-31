#include <allegro5/allegro.h>
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
    // Internal resolution
    320, 200,
    // Frame-rate (FPS)
    40
  };

  if (engine_init(&conf))
  {
    // Run with a starting state
    engine_run(GAME_STATE);
  }

  return 0;
}