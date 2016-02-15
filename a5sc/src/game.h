#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro_font.h>
#include "state.h"

// Color defines
#define C_BLACK     al_map_rgb(0, 0, 0)
#define C_WHITE     al_map_rgb(255, 255, 255)

// These will store the internal resolution (provided in main.c)
int SCREEN_W;
int SCREEN_H;

// Default font as with Allegro 4
ALLEGRO_FONT* font;

struct Game_Config
{
    char *title;
    int width;
    int height;
    int framerate;
    int fullscreen;
    int audio;
};

// Background color
ALLEGRO_COLOR bg_color;

// Main game engine routines
int game_init(struct Game_Config config);
void game_run();
void game_over();

// State routines
void change_state(struct State* state, void* param);
void push_state(struct State* state, void* param);
void pop_state();

// Simple bounding box collision checking (taken from Alex4 source)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
                                                        ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // GAME_H_INCLUDED
