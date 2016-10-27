#include <stdio.h>
#include <allegro.h>
#include "engine.h"
#include "state.h"

const struct Game_Config* maincfg;
static int current_state;

#define SCREEN_RES_OVERRIDE   0.1
// Used to simulate a slightly lower screen resolution
// E.g. without the panels and stuff

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

static void close_button_handler()
{
  is_running = 0;
}
END_OF_FUNCTION(close_button_handler)

// Main game initialization
int game_init(struct Game_Config* config)
{
  if (game.initialized)
  {
    puts("WARNING: Calling game_init() more than once");
    return 1;
  }

  allegro_init();
  install_keyboard();
  install_timer();

  if (config->audio)
  {
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
    {
      puts("WARNING: Could not initialize audio");
    }
  }

  set_color_depth(config->depth);

  if (config->scale <= 0)
  {
    int w, h;
    get_desktop_resolution(&w, &h);

    float new_w = w - (w * SCREEN_RES_OVERRIDE);
    float new_h = h - (h * SCREEN_RES_OVERRIDE);

    config->scale = 2;

    // Keep scaling until a suitable scale factor is found
    while (1)
    {
      int scale_w = config->width * config->scale;
      int scale_h = config->height * config->scale;

      if (scale_w > new_w || scale_h > new_h)
      {
        --config->scale;
        break;
      }

      ++config->scale;
    }
  }
  else if (config->scale < 2)
  {
    config->scale = 2;
  }

  if (set_gfx_mode(GFX_AUTODETECT_WINDOWED,
                   config->width * config->scale,
                   config->height * config->scale,
                   0, 0))
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("ERROR: Could not create a window:\n%s", allegro_error);
    return 0;
  }

  set_window_title(config->title);

  game.buffer = create_bitmap(config->width, config->height);

  set_close_button_callback(close_button_handler);
  set_bg_color(BG_COLOR_DEFAULT);

  maincfg = config;

  game.initialized = TRUE;

  return 1;
}

// Game loop
void game_run(struct State* state, void* param)
{
  int redraw = FALSE;

  if (is_running)
  {
    puts("WARNING: Calling game_run() more than once");
    return;
  }

  change_state(state, param);

  // Main game timer
  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(maincfg->framerate));

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

        stretch_blit(game.buffer, screen,
                     0, 0, GAME_W, GAME_H, 0, 0, SCREEN_W, SCREEN_H);

        ++frame_counter;
      }
    }
    else
    {
      rest(1);
    }
  }

  while (current_state >= 0)
  {
    game.states[current_state--]->_end(TRUE);
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

// Push a new state onto the stack (previous one is 'paused')
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
