#include "Game_Engine.h"
#include "Game_State.h"

int main(int argc, char **argv)
{
  Game_Engine engine;

  bool result = engine.Init(
    // Argument list
    argc, argv,
    // Window title
    "Allegro application",
    // Resolution
    640, 480,
    // Frame-rate (FPS)
    40,
    // Color depth (8, 15, 16, 24, 32)
    8,
    // Full-screen?
    false,
    // Enable mouse?
    false,
    // Enable audio?
    false
  );

  if (result)
  {
    // Run with a starting state
    engine.Run(new Game_State());
  }

  return 0;
}
END_OF_MAIN()
