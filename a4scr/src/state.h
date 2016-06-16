#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro.h>

// Main state structure
struct State
{
  void (*init)(long);
  void (*end)(int);

  void (*pause)();
  void (*resume)();

  void (*update)();
  void (*draw)(BITMAP*);
};

#endif // STATE_H_INCLUDED
