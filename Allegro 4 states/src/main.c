#include <allegro.h>
#include "game.h"
#include "state_game.h"

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

   if (game_init(&conf))
   {  // Run with a starting state
      game_run(STATE_GAME);
   }

   return 0;
}
END_OF_MAIN()
