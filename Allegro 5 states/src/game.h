#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro_font.h>

// Color defines
#define C_BLACK     al_map_rgb(0, 0, 0)
#define C_WHITE     al_map_rgb(255, 255, 255)

#ifndef TRUE
#define FALSE       0
#define TRUE        -1
#endif

// state.h
struct State;

struct Game_Conf
{
   // Argument list
   int argc;
   char **argv;

   // Other needed parameters
   char *title;
   int width;
   int height;
   int framerate;
   int fullscreen;
   int buffer;
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
void set_bg_color(ALLEGRO_COLOR c);

/*****************************************************************************
  Globals
*****************************************************************************/

// Allegro font
extern ALLEGRO_FONT *font;

// Array holding key presses, for state_update().
extern int key[ALLEGRO_KEY_MAX];

// Pointer to the original settings (in main.c).
extern const struct Game_Conf *MAINCONF;

#define GAME_W      MAINCONF->width
#define GAME_H      MAINCONF->height

// Simple bounding box collision checking (taken from Alex4 source)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!(((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
     ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1))))

#endif // GAME_H_INCLUDED
