#include <allegro5/allegro.h>
#include "game_state.h"
#include "engine.h"

/*******************************************************************************
  state_init(param)
  Executed ONLY ONCE by: change_state(), push_state()
*******************************************************************************/
static void
state_init(void *param)
{
}

/*******************************************************************************
  state_end()
  Executed when [engine_active = FALSE]
*******************************************************************************/
static void
state_end(void)
{
}

/*******************************************************************************
  state_enter(param)
  Executed by: change_state(), push_state()
*******************************************************************************/
static void
state_enter(void *param)
{
}

/*******************************************************************************
  state_exit()
  Executed by: change_state(), pop_state()
*******************************************************************************/
static void
state_exit(void)
{
}

/*******************************************************************************
  state_pause()
  Executed by: push_state()
*******************************************************************************/
static void
state_pause(void)
{
}

/*******************************************************************************
  state_resume()
  Executed by: pop_state()
*******************************************************************************/
static void
state_resume(void)
{
}

/*******************************************************************************
  state_events(ev)
  For event handling - keyboard, mouse, joystick, etc.
*******************************************************************************/
static void
state_events(ALLEGRO_EVENT *ev)
{
}

/*******************************************************************************
  state_update()
  Runs every step/frame - for game logic
*******************************************************************************/
static void
state_update(void)
{
}

/*******************************************************************************
  state_draw()
  For drawing to the screen - huds, sprites, backgrounds, etc.
*******************************************************************************/
static void
state_draw(void)
{
}

/*******************************************************************************
  Definition of the state function
*******************************************************************************/
struct State*
Game_State(void)
{
  static struct State state =
  {
    state_init,
    state_end,
    state_enter,
    state_exit,
    state_pause,
    state_resume,
    state_events,
    state_update,
    state_draw
  };

  return &state;
}
