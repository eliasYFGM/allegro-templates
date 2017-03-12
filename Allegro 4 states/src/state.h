#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

// Main state structure
struct State
{
  void (*_init)(void *param);
  void (*_end)(void);

  void (*_pause)(void);
  void (*_resume)(void);

  void (*_update)(void);
  void (*_draw)(BITMAP *dest);
};

#endif // STATE_H_INCLUDED
