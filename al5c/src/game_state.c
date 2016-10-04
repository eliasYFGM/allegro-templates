#include <allegro5/allegro.h>
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

static void state_events(ALLEGRO_EVENT* event)
{
}

static void state_update()
{
}

static void state_draw()
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
    state_events,
    state_update,
    state_draw
  };

  return &state;
}
