#include <stdio.h>
#include <allegro.h>
#include "core.h"
#include "state.h"

struct Game_Config* default_config;
static int current_state;

static struct // Game variables
{
  BITMAP* buffer;
  int initialized;
  int bg_color;
  struct State* states[MAX_STATES];
}
game;

static volatile unsigned int ticks = 0;

static void ticker()
{
  ++ticks;
}
END_OF_FUNCTION(ticker)

volatile int fps = 0;
static volatile int frame_counter = 0;

static void update_fps()
{
  fps = frame_counter;
  frame_counter = 0;
}
END_OF_FUNCTION(update_fps)

static volatile int is_running;

#ifndef ALLEGRO_DOS
static void close_button_handler()
{
  is_running = 0;
}
END_OF_FUNCTION(close_button_handler)
#endif // ALLEGRO_DOS

// Main game initialization
int game_init(struct Game_Config* config)
{
  if (game.initialized)
  {
    puts("WARNING: Game already initialized");
    return 0;
  }

  allegro_init();
  install_keyboard();
  install_mouse();
  install_timer();

  if (config->audio)
  {
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
    {
      puts("WARNING: Could not initialize audio");
    }
  }

  set_color_depth(config->depth);

#ifdef ALLEGRO_DOS
  if (set_gfx_mode(GFX_AUTODETECT, config->width, config->height, 0, 0))
#else
  if (set_gfx_mode(
    config->fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
    config->width, config->height, 0, 0))
#endif
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("ERROR: Could not init graphics mode:\n%s", allegro_error);
    return 0;
  }

#ifndef ALLEGRO_DOS
  set_window_title(config->title);
  set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

  game.buffer = create_bitmap(SCREEN_W, SCREEN_H);
  game.bg_color = makecol(192, 192, 192);
  default_config = config;

  game.initialized = TRUE;

  return 1;
}

// Game loop
void game_run()
{
  int redraw = FALSE;

  if (!game.states[current_state])
  {
    puts("ERROR: change_state was not called");
    return;
  }

  // Main game timer
  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(default_config->framerate));

  // FPS timer
  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(frame_counter);
  LOCK_FUNCTION(update_fps);
  install_int(update_fps, 1000);

  is_running = TRUE;

  // Game loop
  while (is_running)
  {
    if (ticks > 0)
    {
      while (ticks > 0)
      {
        --ticks;

        if (key[KEY_ESC])
        {
          is_running = FALSE;
          break;
        }

        game.states[current_state]->_update();
        redraw = TRUE;
      }

      if (is_running && redraw)
      {
        redraw = FALSE;

        clear_to_color(game.buffer, game.bg_color);

        game.states[current_state]->_draw(game.buffer);

        show_mouse(game.buffer);
        blit(game.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        show_mouse(NULL);

        ++frame_counter;
      }
    }
#ifndef ALLEGRO_DOS
    else
    {
      rest(1);
    }
#endif // ALLEGRO_DOS
  }

  int i;
  for (i=0; i<MAX_STATES; ++i)
  {
    if (game.states[i] != NULL)
    {
      // Always pass TRUE to let the state know that the game is exiting
      game.states[i]->_end(TRUE);
    }
  }

  destroy_bitmap(game.buffer);
}

// Ends the game
void game_over()
{
  is_running = FALSE;
}

void set_bg_color(int color)
{
  game.bg_color = color;
}

// Changes the state directly to another
void change_state(struct State* state, void* param)
{
  if (game.states[current_state] != NULL)
  {
    game.states[current_state]->_end(FALSE);
  }

  game.states[current_state] = state;
  game.states[current_state]->_init(param);
}

// Pushes a new state onto the stack (previous one is 'paused')
void push_state(struct State* state, void* param)
{
  if (current_state < (MAX_STATES - 1))
  {
    if (game.states[current_state] != NULL)
    {
      game.states[current_state]->_pause();
    }

    game.states[++current_state] = state;
    game.states[current_state]->_init(param);
  }
  else
  {
    puts("WARNING: Can't add new state (current_state = MAX_STATES)");
  }
}

// Removes the last state added from the stack
void pop_state()
{
  if (current_state > 0)
  {
    game.states[current_state]->_end(FALSE);
    game.states[current_state] = NULL;
    game.states[--current_state]->_resume();
  }
  else
  {
    puts("WARNING: Can't remove any more states (current_state = 0)");
  }
}
