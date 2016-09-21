#include "Game.h"
#include "GameState.h"

int main()
{
  Game game;

  bool result = game.Init(
    // Window title
    "Allegro application",
    // Screen size
    640, 480,
    // Frame-rate (FPS)
    40,
    // Want full-screen?
    false,
    // Want sound?
    false,
    // Want additional back-buffer? (adds black bars)
    false
  );

  if (result)
  {
    game.Change_State(new Game_State(&game));
    game.Run();
  }

  return 0;
}
