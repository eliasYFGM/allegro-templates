#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

class State
{
public:
  virtual ~State() {}

  virtual void Pause() = 0;
  virtual void Resume() = 0;

  virtual void Events(ALLEGRO_EVENT&) = 0;
  virtual void Update() = 0;
  virtual void Draw() = 0;
};

#endif // STATE_H_INCLUDED
