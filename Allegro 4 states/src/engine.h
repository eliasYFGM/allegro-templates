#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#define C_BLACK     makecol(0, 0, 0)
#define C_WHITE     makecol(255, 255, 255)
#define C_TRANS     makecol(255, 0, 255)

// Max states to allocate for the "loaded_states" stack
#define MAX_STATES  10

// state.h
struct State;

struct Engine_Conf
{
  // Argument list
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

// A state can have an extra parameter [*param] when initialized. It is passed
// to state_load() and state_enter() functions.
struct State_Machine
{
  void (*change_state)(struct State *s, void *param);
  void (*push_state)(struct State *s, void *param);
  void (*pop_state)(void);
};

/*****************************************************************************
  Main
*****************************************************************************/

int engine_init(struct Engine_Conf *conf);
void engine_run(struct State *s);

/*****************************************************************************
  Misc.
*****************************************************************************/

void game_over(void);
void set_bg_color(int c);

/*****************************************************************************
  Globals
*****************************************************************************/

extern volatile int fps;
extern const struct Engine_Conf *MAINCONF;

#define GAME_W      MAINCONF->width  // Internal width
#define GAME_H      MAINCONF->height // Internal height

// Bounding box collision (taken from Alex4)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!(((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
     ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1))))

#endif // ENGINE_H_INCLUDED
