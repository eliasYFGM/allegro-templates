#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro5/allegro.h>

class Game;

class State
{
public:
  virtual ~State() {}

  virtual void Pause(Game*) = 0;
  virtual void Resume(Game*) = 0;

  virtual void Handle_Events(Game*, ALLEGRO_EVENT&) = 0;
  virtual void Update(Game*) = 0;
  virtual void Draw(Game*) = 0;
};

#endif // STATE_H_INCLUDED
