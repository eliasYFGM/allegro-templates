#include <allegro5/allegro.h>
#include "game.h"
#include "gamestate.h"

static void on_init(long param)
{
}

static void on_end(int is_over)
{
}

static void on_pause()
{
}

static void on_resume()
{
}

static void on_events(ALLEGRO_EVENT* event)
{
}

static void on_update()
{
}

static void on_draw()
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
    on_events,
    on_update,
    on_draw
  };

  return &state;
}
