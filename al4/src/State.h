#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro.h>

class Game;

class State
{
public:
  virtual ~State() {}

  virtual void Pause(Game*) = 0;
  virtual void Resume(Game*) = 0;

  virtual void Update(Game*) = 0;
  virtual void Draw(Game*, BITMAP*) = 0;
};

#endif // STATE_H_INCLUDED
