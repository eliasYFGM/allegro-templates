#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

struct State;

// Main state structure
struct State
{
   int (*load)(void *param);
   void (*unload)(void);

   void (*enter)(void *param);
   void (*exit)(void);

   void (*events)(ALLEGRO_EVENT *ev);
   void (*update)(void);
   void (*draw)(void);

   // Private and used only by game.c
   // Indicates if the state was already initialized with load().
   int loaded;
};

#endif // STATE_H_INCLUDED
