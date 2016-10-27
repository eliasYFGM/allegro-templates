#include <allegro.h>
#include "Game_State.h"

int main(int argc, char** argv)
{
  Game_Engine game;

  bool result = game.Init(
    // Argument list
    argc, argv,
    // Window title
    "Allegro application",
    // Screen size
    640, 480,
    // Frame-rate (FPS)
    40,
    // Depth (8, 15, 16, 24, 32)
    8,
    // Want full-screen?
    false,
    // Want audio?
    false
  );

  if (result)
  {
    // Run the game with a default "starting state"
    game.Run(new Game_State());
  }

  return 0;
}
END_OF_MAIN()
