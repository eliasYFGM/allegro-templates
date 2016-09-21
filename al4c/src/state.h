#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro.h>

// Main state structure
struct State
{
  void (*_init)(long);
  void (*_end)(int);

  void (*_pause)();
  void (*_resume)();

  void (*_update)();
  void (*_draw)(BITMAP*);
};

#endif // STATE_H_INCLUDED