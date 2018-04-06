#include <allegro5/allegro.h>
#include "state_game.h"
#include "game.h"

static int state_load(void *param)
{
   // This function must return non-zero on success.
   return 1;
}



static void state_unload(void)
{
}



static void state_enter(void *param)
{
}



static void state_exit(void)
{
}



static void state_events(const ALLEGRO_EVENT *ev)
{
}



static void state_update(const int *key)
{
}



static void state_draw(void)
{
}



struct State *State_Game(void)
{
   static struct State state =
   {  state_load,
      state_unload,
      state_enter,
      state_exit,
      state_events,
      state_update,
      state_draw
   };

   return &state;
}
