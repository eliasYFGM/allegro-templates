#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)
#define C_TRANS   makecol(255, 0, 255)

#define BG_COLOR_DEFAULT  makecol(192, 192, 192)

// Max states to allocate
#define MAX_STATES  8

// Defined in state.h
struct State;

struct Game_Config
{
  char* title;
  int width;
  int height;
  int framerate;
  int depth;
  int fullscreen;
  int audio;
  struct {
    int c;
    char** v;
  } args;
};

// Main game engine routines
int game_init(struct Game_Config*);
void game_run(struct State*);
void game_over();
void set_bg_color(int color);

// State manipulation
void change_state(struct State*, void* param);
void push_state(struct State*, void* param);
void pop_state();

// FPS is updated each second
extern volatile int fps;

// Pointer to the original settings (in main.c)
extern struct Game_Config* main_config;

// Bounding box collision (taken from Alex4)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
      ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // CORE_H_INCLUDED
