#include <stdio.h>
#include <allegro.h>
#include "game.h"
#include "state.h"

static struct // Game data
{
  BITMAP* buffer;
  int initialized;
  int bg_color;
}
game =
{
  NULL,
  0, 0
};

struct Game_Config* default_config = NULL;

static volatile int ticks = 0;

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

#define MAX_STATES  8

// State data
static struct State* states[MAX_STATES];
static int current_state = 0;

// Main game initialization
int game_init(struct Game_Config* config)
{
  int i;

  if (game.initialized)
  {
    puts("WARNING: Game already initialized");
    return 0;
  }

  for (i=0; i<MAX_STATES; ++i)
  {
    states[i] = NULL;
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
#endif // ALLEGRO_DOS

  game.buffer = create_bitmap(SCREEN_W, SCREEN_H);
  game.bg_color = makecol(192, 192, 192);

#ifndef ALLEGRO_DOS
  set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

  game.initialized = TRUE;
  default_config = config;

  return 1;
}

// Game loop
void game_run()
{
  int i, redraw = FALSE;

  if (!states[current_state])
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
    if (ticks)
    {
      while (ticks)
      {
        --ticks;

        if (key[KEY_ESC])
        {
          is_running = FALSE;
          break;
        }

        states[current_state]->update();
        redraw = TRUE;
      }

      if (is_running && redraw)
      {
        redraw = FALSE;

        clear_to_color(game.buffer, game.bg_color);

        states[current_state]->draw(game.buffer);

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

  for (i=0; i<MAX_STATES; ++i)
  {
    if (states[i] != NULL)
    {
      states[i]->end(TRUE);
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
void change_state(struct State* state, long param)
{
  if (states[current_state] != NULL)
  {
    states[current_state]->end(FALSE);
  }

  states[current_state] = state;
  state->init(param);
}

// Pushes a new state onto the stack (previous one is 'paused')
void push_state(struct State* state, long param)
{
  if (current_state < (MAX_STATES - 1))
  {
    if (states[current_state] != NULL)
    {
      states[current_state]->pause();
    }

    states[++current_state] = state;
    state->init(param);
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
    states[current_state]->end(FALSE);
    states[current_state] = NULL;
    states[--current_state]->resume();
  }
  else
  {
    puts("WARNING: Can't remove any more states (current_state = 0)");
  }
}
