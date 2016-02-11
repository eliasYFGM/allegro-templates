#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "state.h"

#define C_BLACK  makecol(0, 0, 0)
#define C_WHITE  makecol(255, 255, 255)

struct Game_Config
{
    int width;
    int height;
    int fullscreen;
    int framerate;
    int depth;
    int audio;
};

// Background color
int bg_color;

// Main functions that control the game
int game_init(struct Game_Config* config, const char* title);
void game_run();
void game_over();

// Main state routines
void change_state(struct State* state, void* param);
void push_state(struct State* state, void* param);
void pop_state();

// Bounding box collision
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
                                                        ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // GAME_H_INCLUDED
