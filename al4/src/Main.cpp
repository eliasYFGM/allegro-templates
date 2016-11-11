#include "Game_Engine.h"
#include "Game_State.h"

int main(int argc, char **argv)
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
    // Full-screen?
    false,
    // Enable mouse?
    false,
    // Enable audio module?
    false
  );

  if (result)
  {
    // Run the game, with a starting state
    game.Run(new Game_State(&game));
  }

  return 0;
}
END_OF_MAIN()
