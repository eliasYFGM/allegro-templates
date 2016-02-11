#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include <allegro.h>

class Game_Engine;

class State
{
public:
    virtual ~State() {}

    virtual void update(Game_Engine* game) = 0;
    virtual void draw(Game_Engine* game, BITMAP* bmp) = 0;

    virtual void pause(Game_Engine* game) = 0;
    virtual void resume(Game_Engine* game) = 0;
};

#endif // STATE_HPP_INCLUDED
