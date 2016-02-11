#ifndef GAMEENGINE_HPP_INCLUDED
#define GAMEENGINE_HPP_INCLUDED

#include <stack>
#include <allegro.h>

#include "state.h"

#define C_BLACK  makecol(0, 0, 0)
#define C_WHITE  makecol(255, 255, 255)

class Game_Engine
{
// Internal game screen
    BITMAP* buffer;

public:
// Main game engine routines
    bool init(const char* title, bool fullscreen, int w, int h, int fps, int depth);
    void run();

// Background color
    int bg_color;

// Is the game actually running?
// Declared as 'static' to let some special functions change it
    static bool is_running;

// State routines
public:
    void change_state(State* state);
    void push_state(State* state);
    void pop_state();

private:
    std::stack<State*> state_list;
};

// Bounding box collision
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
                                                        ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // GAMEENGINE_HPP_INCLUDED
