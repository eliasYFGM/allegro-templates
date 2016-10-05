#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include "State.h"
#include "Game.h"

class Game_State : public State_Object, public Game
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
