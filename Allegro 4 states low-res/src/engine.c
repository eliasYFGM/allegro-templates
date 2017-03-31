#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "engine.h"
#include "state.h"

// Used to simulate a slightly lower monitor resolution
#define SCREEN_RES_OVERRIDE   0.1

const struct Engine_Conf *mainconf;

static struct // Game variables
{
  BITMAP *buffer;
  int initialized;
  int bg_color;
  struct State *states[MAX_STATES];
  struct State *initd_states[MAX_STATES * 2];
} engine;

static int current_state, initd_count;

static volatile unsigned int ticks;

static void ticker(void)
{
  ++ticks;
}
END_OF_FUNCTION(ticker);

volatile int fps;
static volatile int frame_counter;

static void update_fps(void)
{
  fps = frame_counter;
  frame_counter = 0;
}
END_OF_FUNCTION(update_fps);

volatile int engine_active;

static void close_button_handler(void)
{
  engine_active = 0;
}
END_OF_FUNCTION(close_button_handler);

// Amount of scaling
static int scale = 1;

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

  if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
  {
    puts("WARNING: Could not initialize audio");
  }

  set_color_depth(conf->depth);

  int w, h;
  get_desktop_resolution(&w, &h);

  float new_w = w - (w * SCREEN_RES_OVERRIDE);
  float new_h = h - (h * SCREEN_RES_OVERRIDE);

  // Keep scaling until a suitable scale factor is found
  while (1)
  {
    int scale_w = conf->width * scale;
    int scale_h = conf->height * scale;

    if (scale_w > new_w || scale_h > new_h)
    {
      --scale;
      break;
    }

    ++scale;
  }

  if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, conf->width * scale,
    conf->height * scale, 0, 0))
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("ERROR: Could not create a window:\n%s", allegro_error);
    return 0;
  }

  set_window_title(conf->title);

  engine.buffer = create_bitmap(conf->width, conf->height);

  set_close_button_callback(close_button_handler);
  set_bg_color(makecol(192, 192, 192));

  mainconf = conf;

  srand(time(NULL));

  engine.initialized = TRUE;

  return 1;
}

// Game loop
void engine_run(struct State *s)
{
  int redraw = FALSE;

  if (engine_active)
  {
    puts("WARNING: Calling game_run() more than once");
    return;
  }

  change_state(s, NULL);

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

        stretch_blit(engine.buffer, screen,
          0, 0, GAME_W, GAME_H, 0, 0, SCREEN_W, SCREEN_H);

        ++frame_counter;
      }
    }
    else
    {
      rest(1);
    }
  }

  while (initd_count > 0)
  {
    engine.initd_states[--initd_count]->_end();
  }

  destroy_bitmap(engine.buffer);
}

void change_state(struct State *s, void *param)
{
  if (!s->initd)
  {
    s->_init(param);
    s->initd = TRUE;
    engine.initd_states[initd_count++] = s;
  }

  if (engine.states[current_state] != NULL)
  {
    engine.states[current_state]->_exit();
  }

  s->_enter(param);
  engine.states[current_state] = s;

  // Reset tick counter
  ticks = 1;
}

void push_state(struct State *s, void *param)
{
  if (current_state < (MAX_STATES - 1))
  {
    if (!s->initd)
    {
      s->_init(param);
      s->initd = TRUE;
      engine.initd_states[initd_count++] = s;
    }

    if (engine.states[current_state] != NULL)
    {
      engine.states[current_state]->_pause();
    }

    s->_enter(param);
    engine.states[++current_state] = s;

    // Reset tick counter
    ticks = 1;
  }
  else
  {
    puts("WARNING: Can't add new state (current_state = MAX_STATES)");
  }
}

void pop_state(void)
{
  if (current_state > 0)
  {
    engine.states[current_state]->_exit();
    engine.states[--current_state]->_resume();
  }
  else
  {
    puts("WARNING: Can't remove any more states (current_state = 0)");
  }
}

void set_bg_color(int c)
{
  engine.bg_color = c;
}
