#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <allegro.h>
#include "State.h"

class Game_State : public State
{
public:
  void Init();
  void End();

  void Pause();
  void Resume();

  void Update(Game_Engine *engine);
  void Draw(BITMAP *dest);

public:
  static State* Get_State();
};

#define GAME_STATE  Game_State::Get_State()

#endif // GAMESTATE_H_INCLUDED
