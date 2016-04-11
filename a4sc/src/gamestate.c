#include <stdio.h>
#include <allegro.h>
#include "game.h"
#include "gamestate.h"

static void on_init(void* param)
{
}

static void on_end(int over)
{
}

static void on_pause()
{
}

static void on_resume()
{
}

static void on_update()
{
}

static void on_draw(BITMAP* bmp)
{
}

struct State* get_game_state()
{
    static struct State state =
    {
        on_init,
        on_end,
        on_pause,
        on_resume,
        on_update,
        on_draw
    };

    return &state;
}
