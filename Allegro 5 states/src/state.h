#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro5/allegro_font.h>

// Main state structure
struct State
{  int (*load)(void *param);
   void (*unload)(void);

   void (*enter)(void *param);
   void (*exit)(void);

   void (*events)(ALLEGRO_EVENT*);
   void (*update)(int *key);
   void (*draw)(ALLEGRO_FONT*, int fps);

   // Private and used only by [game.c]
   // Indicates if the state was already initialized with load().
   int loaded;
};

#endif // STATE_H_INCLUDED
