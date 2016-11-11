#include <allegro.h>
#include "game_state.h"
#include "engine.h"

static void state_init(void *param)
{
}

static void state_end(int exiting)
{
}

static void state_pause(void)
{
}

static void state_resume(void)
{
}

static void state_update(void)
{
}

static void state_draw(BITMAP *bmp)
{
}

struct State *get_game_state(void)
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
