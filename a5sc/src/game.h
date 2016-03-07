#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro_font.h>
#include "state.h"

// Color defines
#define C_BLACK     al_map_rgb(0, 0, 0)
#define C_WHITE     al_map_rgb(255, 255, 255)

struct Game_Config
{
    char *title;
    int width;
    int height;
    int framerate;
    int fullscreen;
    int audio;
};

extern ALLEGRO_FONT* font;
extern ALLEGRO_COLOR bg_color;

// Main game engine routines
int game_init(struct Game_Config* config);
void game_run();
void game_over();

// Get the (current) display size
int get_screen_width();
int get_screen_height();

#define SCREEN_W    get_screen_width()
#define SCREEN_H    get_screen_height()

// State routines
void change_state(struct State* state, void* param);
void push_state(struct State* state, void* param);
void pop_state();

// Simple bounding box collision checking (taken from Alex4 source)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
                                                        ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // GAME_H_INCLUDED
