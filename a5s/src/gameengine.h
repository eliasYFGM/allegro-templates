#ifndef GAMEENGINE_HPP_INCLUDED
#define GAMEENGINE_HPP_INCLUDED

#include <stack>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "state.h"

// Color facilities
#define C_BLACK  al_map_rgb(0, 0, 0)
#define C_WHITE  al_map_rgb(255, 255, 255)

extern int SCREEN_W;
extern int SCREEN_H;
extern ALLEGRO_FONT* font;

class Game_Engine
{
// Engine variables
    ALLEGRO_DISPLAY* display;
    ALLEGRO_BITMAP* buffer;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* event_queue;

public:
// Main game engine routines
    bool init(const char* title, int w, int h, int rate, bool fullscreen, bool audio);
    void run();

    ALLEGRO_COLOR bg_color;
    bool is_running;

// State routines
public:
    void change_state(State* state);
    void push_state(State* state);
    void pop_state();

private:
    std::stack<State*> state_list;
};

// Simple bounding box collision checking (taken from Alex4 source)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
                                                        ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // GAMEENGINE_HPP_INCLUDED
