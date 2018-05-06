#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

// Main state structure
struct State
{  const char *name;

   int (*On_Load)(void *param);
   void (*On_Unload)(void);

   void (*On_Enter)(void *param);
   void (*On_Exit)(void);

   void (*On_Update)(void);
   void (*On_Draw)(BITMAP *bmp, int fps);

   // Private and used only by [Game.c]
   // Indicates if the state was already initialized with On_Load().
   int loaded;
};

#endif // STATE_H_INCLUDED
