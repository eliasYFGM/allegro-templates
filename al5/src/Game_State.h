#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <allegro5/allegro.h>
#include "State.h"

class Game_State : public State
{
public:
  void Init();
  void End();

  void Pause();
  void Resume();

  void Events(Game_Engine *engine, ALLEGRO_EVENT& ev);
  void Update(Game_Engine *engine);
  void Draw();

public:
  static State* Get_State();
};

#define GAME_STATE  Game_State::Get_State()

#endif // GAMESTATE_H_INCLUDED
