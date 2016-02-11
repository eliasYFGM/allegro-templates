#ifndef GAMESTATE_HPP_INCLUDED
#define GAMESTATE_HPP_INCLUDED

#include "state.h"
#include "gameengine.h"
#include "bitmap.h"

class Game_State : public State
{
public:
    Game_State(Game_Engine* game);
    ~Game_State();

    void update(Game_Engine* game);
    void draw(Game_Engine* game, BITMAP* bmp);

    void pause(Game_Engine* game);
    void resume(Game_Engine* game);
};

#endif // GAMESTATE_HPP_INCLUDED
