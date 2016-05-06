#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include "State.h"

class Game;

class Game_State : public State
{
public:
  Game_State(Game*);
  ~Game_State();

  void Pause(Game*);
  void Resume(Game*);

  void Handle_Events(Game*, ALLEGRO_EVENT&);
  void Update(Game*);
  void Draw(Game*);
};

#endif // GAMESTATE_H_INCLUDED
