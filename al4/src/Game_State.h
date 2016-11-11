#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <allegro.h>
#include "State.h"

class Game_Engine;

class Game_State : public State
{
  Game_Engine *engine;

public:
  Game_State(Game_Engine *ge);
  ~Game_State();

  void Pause();
  void Resume();

  void Update();
  void Draw(BITMAP *bmp);
};

#endif // GAMESTATE_H_INCLUDED
