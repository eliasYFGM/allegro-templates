#include <stdio.h>
#include <allegro.h>
#include "game.h"
#include "gamestate.h"

static void state_init(void* param)
{
}

static void state_end()
{
}

static void state_pause()
{
}

static void state_resume()
{
}

static void state_update()
{
}

static void state_draw(BITMAP* bmp)
{
}

static struct State state;
static int is_new = 1;

struct State* get_game_state()
{
    if (is_new)
    {
        state.init = &state_init;
        state.end = &state_end;
        state.pause = &state_pause;
        state.resume = &state_resume;
        state.update = &state_update;
        state.draw = &state_draw;

        is_new = 0;
    }

    return &state;
}
