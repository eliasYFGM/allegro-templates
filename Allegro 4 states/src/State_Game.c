#include <allegro.h>

#include "State_Game.h"
#include "Game.h"



static int On_Load(void *param)
{  // This function must return non-zero on success.
   return 1;
}



static void On_Unload(void)
{
}



static void On_Enter(void *param)
{
}



static void On_Exit(void)
{
}



static void On_Update(void)
{
}



static void On_Draw(BITMAP *bmp, int fps)
{
}



struct State *State_Game(void)
{  static struct State state =
   {  "Game",
      On_Load,
      On_Unload,
      On_Enter,
      On_Exit,
      On_Update,
      On_Draw
   };

   return &state;
}
