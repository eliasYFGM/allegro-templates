#include <allegro5/allegro.h>
#include "state_game.h"
#include "engine.h"

/*****************************************************************************
  state_load(param) - First state initialization
*****************************************************************************/
static void
state_load( void *param )
{
}

/*****************************************************************************
  state_free() - Shutdown, freeing of state resources
*****************************************************************************/
static void
state_free( void )
{
}

/*****************************************************************************
  state_enter(param) - Normal initialization
*****************************************************************************/
static void
state_enter( void *param )
{
}

/*****************************************************************************
  state_exit() - Normal state exiting
*****************************************************************************/
static void
state_exit( void )
{
}

/*****************************************************************************
  state_pause() - Pause, after a call to push_state()
*****************************************************************************/
static void
state_pause( void )
{
}

/*****************************************************************************
  state_resume() - State continuing, after a call to pop_state()
*****************************************************************************/
static void
state_resume( void )
{
}

/*****************************************************************************
  state_events(ev) - Event handling
*****************************************************************************/
static void
state_events( ALLEGRO_EVENT *ev )
{
}

/*****************************************************************************
  state_update() - Game logic
*****************************************************************************/
static void
state_update( void )
{
}

/*****************************************************************************
  state_draw() - Drawing to the screen
*****************************************************************************/
static void
state_draw( void )
{
}

/*****************************************************************************
  Definition of the state function
*****************************************************************************/
struct State*
State_Game( void )
{
   static struct State state =
   {
      state_load,
      state_free,
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
