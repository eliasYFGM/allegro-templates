#ifndef GAMESTATE_HPP_INCLUDED
#define GAMESTATE_HPP_INCLUDED

#include <allegro5/allegro.h>

#include "state.h"
#include "gameengine.h"
#include "bitmap.h"

class Game_State : public State
{
public:
    Game_State(Game_Engine* game);
    ~Game_State();

    void process_events(Game_Engine* game, ALLEGRO_EVENT& event);
    void update(Game_Engine* game);
    void draw(Game_Engine* game);

    void pause(Game_Engine* game);
    void resume(Game_Engine* game);
};

#endif // GAMESTATE_HPP_INCLUDED
