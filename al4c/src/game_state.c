#include <stdio.h>
#include <allegro.h>
#include "core.h"
#include "game_state.h"

static void state_init(void* param)
{
}

static void state_end(int exiting)
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

struct State* get_game_state()
{
  static struct State state =
  {
    state_init,
    state_end,
    state_pause,
    state_resume,
    state_update,
    state_draw
  };

  return &state;
}
