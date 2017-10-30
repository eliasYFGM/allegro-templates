#include <stdio.h>
#include <allegro.h>
#include "state_game.h"
#include "game.h"

static int
state_load(void *param)
{
   // This function must return non-zero on success.
   return 1;
}

static void
state_unload(void)
{
}

static void
state_enter(void *param)
{
}

static void
state_exit(void)
{
}

static void
state_update(void)
{
}

static void
state_draw(BITMAP *bmp)
{
}

struct State *State_Game(void)
{
   static struct State state =
   {
      state_load,
      state_unload,
      state_enter,
      state_exit,
      state_update,
      state_draw
   };

   return &state;
}
