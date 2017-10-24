#include <stdio.h>
#include <allegro.h>
#include "state_game.h"
#include "engine.h"

/*****************************************************************************
  state_load(param) - First state initialization
*****************************************************************************/
static void
state_load(void *param)
{
}

/*****************************************************************************
  state_free() - Shutdown, freeing state resources
*****************************************************************************/
static void
state_free(void)
{
}

/*****************************************************************************
  state_enter(param) - Normal initialization
*****************************************************************************/
static void
state_enter(void *param)
{
}

/*****************************************************************************
  state_exit() - Normal state exiting
*****************************************************************************/
static void
state_exit(void)
{
}

/*****************************************************************************
  state_pause() - Pause, after a call to push_state()
*****************************************************************************/
static void
state_pause(void)
{
}

/*****************************************************************************
  state_resume() - State continuing, after a call to pop_state()
*****************************************************************************/
static void
state_resume(void)
{
}

/*****************************************************************************
  state_update() - Logic
*****************************************************************************/
static void
state_update(struct State_Machine *sm)
{
}

/*****************************************************************************
  state_draw() - Drawing to the screen
*****************************************************************************/
static void
state_draw(BITMAP *bmp, struct State_Machine *sm)
{
}

/*****************************************************************************
  Definition of the state function
*****************************************************************************/
struct State *State_Game(void)
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
