#include "Game.h"
#include "State_Game.h"

int main(int argc, char **argv)
{  struct Game_Conf conf =
   {  // Argument list
      .argc = argc,
      .argv = argv,

      // Other settings
      .title      = "Allegro application",
      .width      = 640,
      .height     = 480,
      .framerate  = 40,
      .fullscreen = FALSE,
      .buffer     = FALSE
   };

   if (Game_Init(&conf))
   {  // Run with a starting state
      Game_Run(STATE_GAME);
   }

   return 0;
}
