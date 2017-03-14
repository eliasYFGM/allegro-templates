#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

// Main state structure
struct State
{
  void (*_init)(void *param);
  void (*_end)(void);

  void (*_enter)(void *param);
  void (*_exit)(void);

  void (*_pause)(void);
  void (*_resume)(void);

  void (*_events)(ALLEGRO_EVENT *ev);
  void (*_update)(void);
  void (*_draw)(void);

  // Private
  int initd;
};

#endif // STATE_H_INCLUDED
