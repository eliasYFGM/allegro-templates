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

    while (game.Running())
    {
      game.Handle_Events();
      game.Update();
      game.Draw();
    }
  }

  return 0;
}
