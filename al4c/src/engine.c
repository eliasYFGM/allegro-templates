#include <stdio.h>
#include <allegro.h>
#include "engine.h"
#include "state.h"

const struct Engine_Conf *mainconf;

static struct // Engine variables
{
  BITMAP *buffer;
  int initialized;
  int cursor;
  int bg_color;
  struct State *states[MAX_STATES];
}
engine;

static int current_state;

static volatile unsigned int ticks;

static void ticker(void)
{
  ++ticks;
}
END_OF_FUNCTION(ticker)

volatile int fps;
static volatile int frame_counter;

static void update_fps(void)
{
  fps = frame_counter;
  frame_counter = 0;
}
END_OF_FUNCTION(update_fps)

volatile int engine_active;

#ifndef ALLEGRO_DOS
static void close_button_handler(void)
{
  engine_active = 0;
}
END_OF_FUNCTION(close_button_handler)
#endif // ALLEGRO_DOS

// Main game initialization
int engine_init(struct Engine_Conf *conf)
{
  if (engine.initialized)
  {
    puts("WARNING: Calling game_init() more than once");
    return 1;
  }

  allegro_init();
  install_keyboard();
  install_timer();

  if (conf->mouse)
  {
    install_mouse();
  }

  if (conf->audio)
  {
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
    {
      puts("WARNING: Could not initialize audio");
    }
  }

  set_color_depth(conf->depth);

#ifdef ALLEGRO_DOS
  if (set_gfx_mode(GFX_AUTODETECT, conf->width, conf->height, 0, 0))
#else
  if (set_gfx_mode(
    conf->fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
    conf->width, conf->height, 0, 0))
#endif
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("ERROR: Could not init graphics mode:\n%s", allegro_error);
    return 0;
  }

#ifndef ALLEGRO_DOS
  set_window_title(conf->title);
  set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

  engine.buffer = create_bitmap(SCREEN_W, SCREEN_H);
  engine.bg_color = BG_COLOR_DEFAULT;
  engine.cursor = conf->mouse;

  mainconf = conf;

  engine.initialized = TRUE;

  return 1;
}

// Game loop
void engine_run(struct State *first)
{
  int redraw = FALSE;

  if (engine_active)
  {
    puts("WARNING: Calling game_run() more than once");
    return;
  }

  change_state(first);

  // Main game timer
  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(mainconf->framerate));

  // FPS timer
  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(frame_counter);
  LOCK_FUNCTION(update_fps);
  install_int(update_fps, 1000);

  engine_active = TRUE;

  // Game loop
  while (engine_active)
  {
    if (ticks > 0)
    {
      while (ticks > 0)
      {
        --ticks;

        if (key[KEY_ESC])
        {
          engine_active = FALSE;
          break;
        }

        engine.states[current_state]->_update();
        redraw = TRUE;
      }

      if (engine_active && redraw)
      {
        redraw = FALSE;

        clear_to_color(engine.buffer, engine.bg_color);

        engine.states[current_state]->_draw(engine.buffer);

        if (mainconf->mouse && engine.cursor)
        {
          show_mouse(engine.buffer);
        }

        blit(engine.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (mainconf->mouse && engine.cursor)
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
    engine.states[current_state--]->_end();
  }

  destroy_bitmap(engine.buffer);
}

// Changes the state directly to another
void change_state(struct State *s)
{
  if (engine.states[current_state] != NULL)
  {
    engine.states[current_state]->_end();
  }

  engine.states[current_state] = s;
}

// Push a new state onto the stack (previous one is 'paused')
void push_state(struct State *s)
{
  if (current_state < (MAX_STATES - 1))
  {
    if (engine.states[current_state] != NULL)
    {
      engine.states[current_state]->_pause();
    }

    engine.states[++current_state] = s;
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
    engine.states[current_state]->_end();
    engine.states[--current_state]->_resume();
  }
  else
  {
    puts("WARNING: Can't remove any more states (current_state = 0)");
  }
}

void enable_cursor(int enable)
{
  engine.cursor = enable;
}

void set_bg_color(int c)
{
  engine.bg_color = c;
}
