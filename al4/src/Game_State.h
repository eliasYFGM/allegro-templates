#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <allegro.h>
#include "State.h"

class Game_State : public State
{
public:
  Game_State();
  ~Game_State();

  void Pause();
  void Resume();

  void Update(Game_Engine *engine);
  void Draw(BITMAP *bmp);
};

#endif // GAMESTATE_H_INCLUDED
