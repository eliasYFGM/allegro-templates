#include <allegro5/allegro.h>
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

static void state_events(ALLEGRO_EVENT* event)
{
}

static void state_update()
{
}

static void state_draw()
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
        state.events = &state_events;
        state.update = &state_update;
        state.draw = &state_draw;

        is_new = 0;
    }

    return &state;
}
