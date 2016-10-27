#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

// Main state structure
struct State
{
  void (*_init)(void*);
  void (*_end)(int);

  void (*_pause)();
  void (*_resume)();

  void (*_update)();
  void (*_draw)(BITMAP*);
};

#endif // STATE_H_INCLUDED
