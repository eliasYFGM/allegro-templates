#include <allegro.h>
#include "game_state.h"
#include "engine.h"

static void state_init(void)
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

struct State *game_state(void)
{
  static struct State state =
  {
    state_end,
    state_pause,
    state_resume,
    state_update,
    state_draw
  };

  state_init();

  return &state;
}
