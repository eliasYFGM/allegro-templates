#include "Game.h"
#include "GameState.h"

int main()
{
  Game game;

  bool result = game.Init(
    // Screen size
    640, 480,
    // Window title
    "Game",
    // Full-screen
    false,
    // Fixed rate (FPS)
    40,
    // Depth (8, 15, 16, 24, 32)
    8
  );

  if (result)
  {
    game.Change_State(new Game_State(&game));
    game.Run();
  }

  return 0;
}
END_OF_MAIN()
