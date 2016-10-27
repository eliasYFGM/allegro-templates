#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include "State.h"
#include "Game_Engine.h"

class Game_State : public State, public Game_Engine
{
public:
  Game_State();
  ~Game_State();

  void Pause();
  void Resume();

  void Update();
  void Draw(BITMAP*);
};

#endif // GAMESTATE_H_INCLUDED
