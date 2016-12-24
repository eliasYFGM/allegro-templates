#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

class Game_Engine;

class State
{
public:
  virtual ~State() {}

  virtual void Pause() = 0;
  virtual void Resume() = 0;

  virtual void Update(Game_Engine *engine) = 0;
  virtual void Draw(BITMAP *bmp) = 0;
};

#endif // STATE_H_INCLUDED
