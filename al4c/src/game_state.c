#include <allegro.h>
#include "core.h"
#include "game_state.h"

static void game_state_init(long param)
{
}

static void game_state_end(int exiting)
{
}

static void game_state_pause()
{
}

static void game_state_resume()
{
}

static void game_state_update()
{
}

static void game_state_draw(BITMAP* bmp)
{
}

struct State* get_game_state()
{
  static struct State state =
  {
    game_state_init,
    game_state_end,
    game_state_pause,
    game_state_resume,
    game_state_update,
    game_state_draw
  };

  return &state;
}
