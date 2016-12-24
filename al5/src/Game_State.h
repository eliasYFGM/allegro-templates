#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <allegro5/allegro.h>
#include "State.h"

class Game_State : public State
{
public:
  Game_State();
  ~Game_State();

  void Pause();
  void Resume();

  void Events(Game_Engine *engine, ALLEGRO_EVENT& ev);
  void Update(Game_Engine *engine);
  void Draw();
};

#endif // GAMESTATE_H_INCLUDED
