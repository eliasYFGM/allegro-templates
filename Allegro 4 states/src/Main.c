#include <allegro.h>

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
      .depth      = 8,
      .fullscreen = FALSE
   };

   if (Game_Init(&conf))
   {  Game_Run(STATE_GAME); // Run with a starting state
   }

   return 0;
}
END_OF_MAIN()
