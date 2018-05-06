#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define C_BLACK     makecol(0, 0, 0)
#define C_WHITE     makecol(255, 255, 255)


// state.h
struct State;


struct Game_Conf
{  // Argument list
   int argc;
   char **argv;

   // Other needed parameters
   char *title;
   int width;
   int height;
   int framerate;
   int depth;
   int fullscreen;
};



/*****************************************************************************
  Main
*****************************************************************************/

int Game_Init(struct Game_Conf *conf);
void Game_Run(struct State *s);



/*****************************************************************************
  Misc.
*****************************************************************************/

int Change_State(struct State *s, void *param);
void Game_Over(void);
void Set_BG_Color(int c);
int Make_Transparent(void);



/*****************************************************************************
  Global(s)
*****************************************************************************/

// Global pointer to the main configuration
extern struct Game_Conf *mainconf;



#define GAME_W      mainconf->width  // Internal width
#define GAME_H      mainconf->height // Internal height

// Bounding box collision (taken from Alex4)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!(((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
     ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1))))

#endif // GAME_H_INCLUDED
