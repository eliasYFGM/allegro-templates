#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro5/allegro_font.h>

// Main state structure
struct State
{  const char *name;

   int (*On_Load)(void *param);
   void (*On_Unload)(void);

   void (*On_Enter)(void *param);
   void (*On_Exit)(void);

   void (*On_Event)(ALLEGRO_EVENT*);
   void (*On_Update)(int *key);
   void (*On_Draw)(ALLEGRO_FONT*, int fps);

   // Private and used only by [Game.c]
   // Indicates if the state was already initialized with load().
   int loaded;
};

#endif // STATE_H_INCLUDED
