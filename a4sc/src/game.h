#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define C_BLACK  makecol(0, 0, 0)
#define C_WHITE  makecol(255, 255, 255)

struct Game_Config
{
  char *title;
  int width;
  int height;
  int framerate;
  int depth;
  int fullscreen;
  int audio;
};

// Pointer to the original settings (given in main.c)
extern struct Game_Config* default_config;

// FPS is updated each second
extern volatile int fps;

// Main game engine routines
int game_init(struct Game_Config* config);
void game_run();
void game_over();
void set_bg_color(int color);

struct State;

// State routines
void change_state(struct State* state, long param);
void push_state(struct State* state, long param);
void pop_state();

// Bounding box collision (taken from Alex4)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
      ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // GAME_H_INCLUDED
