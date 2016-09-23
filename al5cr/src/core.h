#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include <allegro5/allegro_font.h>

// Color defines
#define C_BLACK     al_map_rgb(0, 0, 0)
#define C_WHITE     al_map_rgb(255, 255, 255)

#ifndef TRUE
#define FALSE   0
#define TRUE    -1
#endif

// Max states to allocate
#define MAX_STATES  8

struct Game_Config
{
  char *title;
  int width;
  int height;
  int scale;
  int framerate;
  int audio;
};

// Pointer to the original settings (given in main.c)
extern struct Game_Config* default_config;

#define GAME_W    default_config->width
#define GAME_H    default_config->height

// Default fixed-width font
extern ALLEGRO_FONT* font;

// Array holding key presses; only for the state's _update() function
extern int keys[ALLEGRO_KEY_MAX];

// Main game engine routines
int game_init(struct Game_Config* config);
void game_run();
void game_over(); // Only meant for states; NO need to call after "game_run"
void set_bg_color(ALLEGRO_COLOR);

struct State;

// State routines
void change_state(struct State* state, long param);
void push_state(struct State* state, long param);
void pop_state();

// Simple bounding box collision checking (taken from Alex4 source)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
      ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // CORE_H_INCLUDED
