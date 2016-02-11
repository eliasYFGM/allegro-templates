#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include <allegro5/allegro.h>

class Game_Engine;

class State
{
public:
    virtual ~State() {}

    virtual void process_events(Game_Engine* game, ALLEGRO_EVENT& event) = 0;
    virtual void update(Game_Engine* game) = 0;
    virtual void draw(Game_Engine* game) = 0;

    virtual void pause(Game_Engine* game) = 0;
    virtual void resume(Game_Engine* game) = 0;
};

#endif // STATE_HPP_INCLUDED
