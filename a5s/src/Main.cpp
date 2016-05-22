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
    40
  );

  if (result)
  {
    game.Change_State(new Game_State(&game));
    game.Run();
  }

  return 0;
}
