#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include <allegro5/allegro.h>
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

  void Events(ALLEGRO_EVENT &ev);
  void Update();
  void Draw();
};

#endif // GAMESTATE_H_INCLUDED
