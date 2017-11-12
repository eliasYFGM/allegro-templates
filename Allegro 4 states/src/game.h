#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define C_BLACK     makecol(0, 0, 0)
#define C_WHITE     makecol(255, 255, 255)
#define C_TRANS     makecol(255, 0, 255)

// state.h
struct State;

struct Game_Conf
{
   // Argument list
   int argc; char **argv;

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

int game_init(struct Game_Conf *conf);
void game_run(struct State *s);

/*****************************************************************************
  Misc.
*****************************************************************************/

int change_state(struct State *s, void *param);
void game_over(void);
void set_bg_color(int c);

/*****************************************************************************
  Globals
*****************************************************************************/

extern volatile int fps;
extern struct Game_Conf *mainconf;

#define GAME_W      mainconf->width  // Internal width
#define GAME_H      mainconf->height // Internal height

// Bounding box collision (taken from Alex4)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!(((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
     ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1))))

#endif // GAME_H_INCLUDED
