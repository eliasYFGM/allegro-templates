#include <stdio.h>
#include <allegro.h>
#include "engine.h"
#include "state.h"

const struct Game_Config *maincfg;

static int current_state;

static struct // Game variables
{
  BITMAP *buffer;
  int initialized;
  int cursor;
  int bg_color;
  struct State *states[MAX_STATES];
}
game;

static volatile unsigned int ticks = 0;

static void ticker(void)
{
  ++ticks;
}
END_OF_FUNCTION(ticker)

volatile int fps = 0;
static volatile int frame_counter = 0;

static void update_fps(void)
{
  fps = frame_counter;
  frame_counter = 0;
}
END_OF_FUNCTION(update_fps)

static volatile int is_running;

#ifndef ALLEGRO_DOS
static void close_button_handler(void)
{
  is_running = 0;
}
END_OF_FUNCTION(close_button_handler)
#endif // ALLEGRO_DOS

// Main game initialization
int game_init(struct Game_Config *cfg)
{
  if (game.initialized)
  {
    puts("WARNING: Calling game_init() more than once");
    return 1;
  }

  allegro_init();
  install_keyboard();
  install_timer();

  if (cfg->mouse)
  {
    install_mouse();
  }

  if (cfg->audio)
  {
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
    {
      puts("WARNING: Could not initialize audio");
    }
  }

  set_color_depth(cfg->depth);

#ifdef ALLEGRO_DOS
  if (set_gfx_mode(GFX_AUTODETECT, cfg->width, cfg->height, 0, 0))
#else
  if (set_gfx_mode(
    cfg->fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
    cfg->width, cfg->height, 0, 0))
#endif
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("ERROR: Could not init graphics mode:\n%s", allegro_error);
    return 0;
  }

#ifndef ALLEGRO_DOS
  set_window_title(cfg->title);
  set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

  game.buffer = create_bitmap(SCREEN_W, SCREEN_H);
  game.bg_color = BG_COLOR_DEFAULT;
  game.cursor = cfg->mouse;

  maincfg = cfg;

  game.initialized = TRUE;

  return 1;
}

// Game loop
void game_run(struct State *first, void *param)
{
  int redraw = FALSE;

  if (is_running)
  {
    puts("WARNING: Calling game_run() more than once");
    return;
  }

  change_state(first, param);

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

        if (maincfg->mouse && game.cursor)
        {
          show_mouse(game.buffer);
        }

        blit(game.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (maincfg->mouse && game.cursor)
        {
          show_mouse(NULL);
        }

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

  while (current_state >= 0)
  {
    // Always pass TRUE to the state when the game is exiting
    game.states[current_state--]->_end(TRUE);
  }

  destroy_bitmap(game.buffer);
}

// Changes the state directly to another
void change_state(struct State *s, void *param)
{
  if (game.states[current_state] != NULL)
  {
    game.states[current_state]->_end(FALSE);
  }

  game.states[current_state] = s;
  game.states[current_state]->_init(param);
}

// Push a new state onto the stack (previous one is 'paused')
void push_state(struct State *s, void *param)
{
  if (current_state < (MAX_STATES - 1))
  {
    if (game.states[current_state] != NULL)
    {
      game.states[current_state]->_pause();
    }

    game.states[++current_state] = s;
    game.states[current_state]->_init(param);
  }
  else
  {
    puts("WARNING: Can't add new state (current_state = MAX_STATES)");
  }
}

// Removes the last state added from the stack
void pop_state(void)
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

// Ends the game
void game_over(void)
{
  is_running = FALSE;
}

void enable_cursor(int enable)
{
  game.cursor = enable;
}

void set_bg_color(int c)
{
  game.bg_color = c;
}