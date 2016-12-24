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
    // Full-screen?
    false,
    // Enable audio?
    false,
    // Additional back-buffer? (adds black bars on full-screen)
    false
  );

  if (result)
  {
    // Run with a starting state
    engine.Run(new Game_State());
  }

  return 0;
}
