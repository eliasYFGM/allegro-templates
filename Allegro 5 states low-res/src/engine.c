#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include "engine.h"
#include "state.h"

// Used to simulate a slightly lower monitor resolution
#define SCREEN_RES_OVERRIDE 0.1

// Globals
ALLEGRO_FONT *font;
int keys[ALLEGRO_KEY_MAX];
const struct Engine_Conf *mainconf;

static struct // Engine data
  {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_COLOR bg_color;
    int initialized;

    // Stack of states
    struct State *states[MAX_STATES * 2];

    // Initialized states
    struct State *loaded_states[MAX_STATES];
  }
engine;

static int current_state, loaded_count, engine_active;

// Scale factor
static int scale = 1;

int engine_init(struct Engine_Conf *conf)
{
  if (engine.initialized)
    {
      return 1;
    }

  // Initialize Allegro and stuff
  al_init();

  if (!al_install_keyboard())
    {
      puts("engine_init(): Failed to initialize the keyboard...");
      return 0;
    }

  if (al_install_audio())
    {
      if (!al_init_acodec_addon())
        {
          puts("engine_init(): Failed to initialize codecs...");
        }
    }
  else
    {
      puts("engine_init(): Failed to initialize audio...");
    }

  // Add-ons
  if (!al_init_image_addon())
    {
      puts("engine_init(): Failed to initialize image addon...");
      return 0;
    }

  al_init_font_addon();
  al_init_primitives_addon();

  // Find how much the game will be scaled
  ALLEGRO_MONITOR_INFO info;
  al_get_monitor_info(0, &info);

  int monitor_w = info.x2 - info.x1;
  int monitor_h = info.y2 - info.y1;

  int new_monitor_w = (monitor_w - (monitor_w * SCREEN_RES_OVERRIDE));
  int new_monitor_h = (monitor_h - (monitor_h * SCREEN_RES_OVERRIDE));

  // Keep scaling until a suitable scale factor is found
  while (1)
    {
      int scale_w = conf->width * scale;
      int scale_h = conf->height * scale;

      if (scale_w > new_monitor_w || scale_h > new_monitor_h)
        {
          --scale;
          break;
        }

      ++scale;
    }

  engine.display = al_create_display(conf->width * scale,
                                     conf->height * scale);

  if (!engine.display)
    {
      puts("engine_init(): Failed to create a display window...");
      return 0;
    }

  al_set_window_title(engine.display, conf->title);

  font = al_create_builtin_font();

  engine.timer = al_create_timer(1.0 / conf->framerate);
  engine.event_queue = al_create_event_queue();

  mainconf = conf;
  set_bg_color(al_map_rgb(192, 192, 192));

  ALLEGRO_TRANSFORM trans;
  al_identity_transform(&trans);
  al_scale_transform(&trans, scale, scale);
  al_use_transform(&trans);

  srand(time(NULL));

  engine.initialized = TRUE;

  return 1;
}

void engine_run(struct State *s)
{
  int redraw = FALSE;

  if (engine_active)
    {
      return;
    }

  change_state(s, NULL);

  // Generate display events
  al_register_event_source(engine.event_queue,
                           al_get_display_event_source(engine.display));

  // Timer events
  al_register_event_source(engine.event_queue,
                           al_get_timer_event_source(engine.timer));

  // Keyboard events
  al_register_event_source(engine.event_queue,
                           al_get_keyboard_event_source());

  engine_active = TRUE;

  // Main game loop
  while (engine_active)
    {
      ALLEGRO_EVENT event;
      al_wait_for_event(engine.event_queue, &event);

      // Event processing
      engine.states[current_state]->_events(&event);

      // If the close button was pressed...
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
          engine_active = FALSE;
          break;
        }
      else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
          keys[event.keyboard.keycode] = TRUE;
        }
      else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
          keys[event.keyboard.keycode] = FALSE;
        }
      else if (event.type == ALLEGRO_EVENT_TIMER)
        {
          engine.states[current_state]->_update();
          redraw = TRUE;
        }

      if (redraw && al_is_event_queue_empty(engine.event_queue))
        {
          redraw = FALSE;

          al_set_target_backbuffer(engine.display);

          al_clear_to_color(engine.bg_color);

          engine.states[current_state]->_draw();

          al_flip_display();
        }
    }

  while (loaded_count > 0)
    {
      engine.loaded_states[--loaded_count]->_free();
    }

  al_destroy_display(engine.display);
  al_destroy_timer(engine.timer);
  al_destroy_event_queue(engine.event_queue);
  al_destroy_font(font);
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

  al_stop_timer(engine.timer);

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
          al_start_timer(engine.timer);
          return;
        }
    }

  if (engine.states[current_state] != NULL)
    {
      engine.states[current_state]->_exit();
    }

  s->_enter(param);
  engine.states[current_state] = s;

  al_start_timer(engine.timer);

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

      al_stop_timer(engine.timer);

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
              al_start_timer(engine.timer);
              return;
            }
        }

      if (engine.states[current_state] != NULL)
        {
          engine.states[current_state]->_pause();
        }

      s->_enter(param);
      engine.states[++current_state] = s;

      al_start_timer(engine.timer);

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

void set_bg_color(ALLEGRO_COLOR c)
{
  engine.bg_color = c;
}
