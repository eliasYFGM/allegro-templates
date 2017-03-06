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

#define SCREEN_RES_OVERRIDE   0.1
// Used to simulate a slightly lower screen resolution
// E.g. without the panels and stuff

// Globals
int engine_active;
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
  struct State *states[MAX_STATES];
}
engine;

// The state that is currently updating
static int current_state;

int engine_init(struct Engine_Conf *conf)
{
  if (engine.initialized)
  {
    puts("WARNING: Calling game_init() more than once");
    return 1;
  }

  // Initialize Allegro and stuff
  al_init();

  if (!al_install_keyboard())
  {
    puts("ERROR: Could not initialize the keyboard...");
    return 0;
  }

  if (conf->audio)
  {
    if (!al_install_audio())
    {
      puts("ERROR: Could not initialize audio...");
      return 0;
    }

    if (!al_init_acodec_addon())
    {
      puts("ERROR: Could not initialize acodec addon...");
      return 0;
    }
  }

  // Add-ons
  if (!al_init_image_addon())
  {
    puts("ERROR: Could not initialize image addon...");
    return 0;
  }

  al_init_font_addon();
  al_init_primitives_addon();

  // Find how much the game will be scaled when conf->scale <= 0
  if (conf->scale <= 0)
  {
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);

    int monitor_w = info.x2 - info.x1;
    int monitor_h = info.y2 - info.y1;

    float new_monitor_w = (monitor_w - (monitor_w * SCREEN_RES_OVERRIDE));
    float new_monitor_h = (monitor_h - (monitor_h * SCREEN_RES_OVERRIDE));

    conf->scale = 2;

    // Keep scaling until a suitable scale factor is found
    while (1)
    {
      int scale_w = conf->width * conf->scale;
      int scale_h = conf->height * conf->scale;

      if (scale_w > new_monitor_w || scale_h > new_monitor_h)
      {
        --conf->scale;
        break;
      }

      ++conf->scale;
    }
  }
  else if (conf->scale < 2)
  {
    conf->scale = 2;
  }

  engine.display = al_create_display(conf->width * conf->scale,
    conf->height * conf->scale);

  if (!engine.display)
  {
    puts("ERROR: Could not create a display window...");
    return 0;
  }

  al_set_window_title(engine.display, conf->title);

  font = al_create_builtin_font();

  engine.timer = al_create_timer(1.0 / conf->framerate);
  engine.event_queue = al_create_event_queue();

  mainconf = conf;
  set_bg_color(BG_COLOR_DEFAULT);

  ALLEGRO_TRANSFORM trans;
  al_identity_transform(&trans);
  al_scale_transform(&trans, conf->scale, conf->scale);
  al_use_transform(&trans);

  srand(time(NULL));

  engine.initialized = TRUE;

  return 1;
}

void engine_run(struct State *first)
{
  int redraw = FALSE;

  if (engine_active)
  {
    puts("WARNING: Calling game_run() more than once");
    return;
  }

  change_state(first, NULL);

  // Generate display events
  al_register_event_source(engine.event_queue,
    al_get_display_event_source(engine.display));

  // Timer events
  al_register_event_source(engine.event_queue,
    al_get_timer_event_source(engine.timer));

  // Keyboard events
  al_register_event_source(engine.event_queue, al_get_keyboard_event_source());

  al_start_timer(engine.timer);
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

      // Escape key will end the game
      if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
      {
        engine_active = FALSE;
        break;
      }
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

    if (redraw && al_event_queue_is_empty(engine.event_queue))
    {
      redraw = FALSE;

      al_set_target_backbuffer(engine.display);

      al_clear_to_color(engine.bg_color);

      engine.states[current_state]->_draw();

      al_flip_display();
    }
  }

  while (current_state >= 0)
  {
    engine.states[current_state--]->_end();
  }

  al_destroy_display(engine.display);
  al_destroy_timer(engine.timer);
  al_destroy_event_queue(engine.event_queue);
  al_destroy_font(font);
}

void change_state(struct State *s, void *param)
{
  if (engine.states[current_state] != NULL)
  {
    engine.states[current_state]->_end();
  }

  engine.states[current_state] = s;
  engine.states[current_state]->_init(param);
}

void push_state(struct State* s, void *param)
{
  if (current_state < (MAX_STATES - 1))
  {
    if (engine.states[current_state] != NULL)
    {
      engine.states[current_state]->_pause();
    }

    engine.states[++current_state] = s;
    engine.states[current_state]->_init(param);
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
    engine.states[current_state]->_end();
    engine.states[--current_state]->_resume();
  }
  else
  {
    puts("WARNING: Can't remove any more states (current_state = 0)");
  }
}

void set_bg_color(ALLEGRO_COLOR c)
{
  engine.bg_color = c;
}
