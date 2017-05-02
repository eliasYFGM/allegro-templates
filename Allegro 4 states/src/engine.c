#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "engine.h"
#include "state.h"

// Used to simulate a slightly lower monitor resolution
#define SCREEN_RES_OVERRIDE 0.1

const struct Engine_Conf *MAINCONF;

static struct // Engine variables
  {
    BITMAP *buffer;
    int initialized;
    int bg_color;

    // Stack of states
    struct State *states[MAX_STATES * 2];

    // Initialized states
    struct State *loaded_states[MAX_STATES];
  }
engine;

static int current_state, loaded_count;

static volatile int ticks;
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

static volatile int engine_active;

#ifndef ALLEGRO_DOS
static void close_button_handler(void)
{
  engine_active = FALSE;
}
END_OF_FUNCTION(close_button_handler);

// Amount of scaling
static int scale = 1;

#endif // ALLEGRO_DOS

// Main game initialization
int engine_init(struct Engine_Conf *conf)
{
  if (engine.initialized)
    {
      return 1;
    }

  allegro_init();
  install_keyboard();
  install_timer();

  if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
    {
      puts("engine_init(): Failed to initialize audio");
    }

  set_color_depth(conf->depth);

#ifdef ALLEGRO_DOS
  if (set_gfx_mode(GFX_AUTODETECT, conf->width, conf->height, 0, 0))
#else
  if (!conf->fullscreen)
    {
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

      if (!scale)
        {
          scale = 1;
        }
    }

  if (set_gfx_mode(conf->fullscreen ? GFX_AUTODETECT : GFX_AUTODETECT_WINDOWED,
                   conf->width * scale, conf->height * scale, 0, 0))
#endif // ALLEGRO_DOS
    {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("engine_init(): Failed to create a window\n%s",
                      allegro_error);
      return 0;
    }

#ifndef ALLEGRO_DOS
  set_window_title(conf->title);
  set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

  engine.buffer = create_bitmap(conf->width, conf->height);

  set_bg_color(makecol(192, 192, 192));

  MAINCONF = conf;

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
      return;
    }

  change_state(s, NULL);

  // Main game timer
  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(MAINCONF->framerate));

  // FPS timer
  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(frame_counter);
  LOCK_FUNCTION(update_fps);
  install_int(update_fps, 1000);

  engine_active = TRUE;

  // Game loop
  while (engine_active)
    {
      while (ticks > 0)
        {
          --ticks;

          if (key[KEY_ALT] && key[KEY_F4])
            {
              game_over();
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

#ifdef ALLEGRO_DOS
          blit(engine.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
#else
          if (scale > 1)
            {
              stretch_blit(engine.buffer, screen, 0, 0, GAME_W, GAME_H, 0, 0,
                           SCREEN_W, SCREEN_H);
            }
          else
            {
              blit(engine.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
            }
#endif // ALLEGRO_DOS

          ++frame_counter;
        }

#ifndef ALLEGRO_DOS
      rest(1);
#endif // ALLEGRO_DOS
    }

  while (loaded_count > 0)
    {
      engine.loaded_states[--loaded_count]->_free();
    }

  destroy_bitmap(engine.buffer);
}

static int can_change = TRUE;

void change_state(struct State *s, void *param)
{
  if (!can_change)
    {
      puts("change_state(): A thread is already running...");
      return;
    }

  can_change = FALSE;

  if (!s->loaded)
    {
      if (loaded_count < MAX_STATES - 1)
        {
          s->_load(param);
          s->loaded = TRUE;
          engine.loaded_states[loaded_count++] = s;
        }
      else
        {
          puts("change_state(): Reached MAX_STATES");
          return;
        }
    }

  if (engine.states[current_state] != NULL)
    {
      engine.states[current_state]->_exit();
    }

  s->_enter(param);
  engine.states[current_state] = s;

  // Reset tick counter
  ticks = 1;

  can_change = TRUE;
}

void push_state(struct State *s, void *param)
{
  if (current_state < (MAX_STATES * 2) - 1)
    {
      if (!can_change)
        {
          puts("push_state(): A thread is already running...");
          return;
        }

      can_change = FALSE;

      if (!s->loaded)
        {
          if (loaded_count < MAX_STATES - 1)
            {
              s->_load(param);
              s->loaded = TRUE;
              engine.loaded_states[loaded_count++] = s;
            }
          else
            {
              puts("push_state(): Reached MAX_STATES");
              return;
            }
        }

      if (engine.states[current_state] != NULL)
        {
          engine.states[current_state]->_pause();
        }

      s->_enter(param);
      engine.states[++current_state] = s;

      // Reset tick counter
      ticks = 1;

      can_change = TRUE;
    }
  else
    {
      puts("push_state(): State stack is full");
    }
}

void pop_state(void)
{
  if (current_state > 0)
    {
      engine.states[current_state]->_exit();
      engine.states[--current_state]->_resume();
    }
}

void game_over(void)
{
  engine_active = FALSE;
}

void set_bg_color(int c)
{
  engine.bg_color = c;
}
