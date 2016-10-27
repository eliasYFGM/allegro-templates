#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <allegro5/allegro.h>
#include "State.h"
#include "Game_Engine.h"

class Game_State : public State, public Game_Engine
{
public:
  Game_State();
  ~Game_State();

  void Pause();
  void Resume();

  void Events(ALLEGRO_EVENT&);
  void Update();
  void Draw();
};

#endif // GAMESTATE_H_INCLUDED
