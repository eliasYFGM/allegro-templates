#include <allegro.h>
#include "state_game.h"
#include "engine.h"

/*******************************************************************************
  state_load(param) - First state initialization
  Executed ONLY ONCE by: change_state(), push_state()
*******************************************************************************/
static void
state_load(void *param)
{
}

/*******************************************************************************
  state_free() - Shutdown, freeing of state resources
  Executed after a call to game_over()
*******************************************************************************/
static void
state_free(void)
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
state_draw(BITMAP *bmp)
{
}

/*******************************************************************************
  Definition of the state function
*******************************************************************************/
struct State*
State_Game(void)
{
  static struct State state =
    {
      state_load,
      state_free,
      state_enter,
      state_exit,
      state_pause,
      state_resume,
      state_update,
      state_draw
    };

  return &state;
}
