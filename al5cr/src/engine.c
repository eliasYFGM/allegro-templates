#include <stdio.h>
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
const struct Game_Config *maincfg;
ALLEGRO_FONT *font;
int keys[ALLEGRO_KEY_MAX];

// The state that is currently updating
static int current_state;

static struct // Engine data
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_COLOR bg_color;
  int initialized;
  int is_running;
  struct State *states[MAX_STATES];
}
game;

int game_init(struct Game_Config *cfg)
{
  if (game.initialized)
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

  if (cfg->audio)
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

  // Find how much the game will be scaled when cfg->scale <= 0
  if (cfg->scale <= 0)
  {
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);

    int monitor_w = info.x2 - info.x1;
    int monitor_h = info.y2 - info.y1;

    float new_monitor_w = (monitor_w - (monitor_w * SCREEN_RES_OVERRIDE));
    float new_monitor_h = (monitor_h - (monitor_h * SCREEN_RES_OVERRIDE));

    cfg->scale = 2;

    // Keep scaling until a suitable scale factor is found
    while (1)
    {
      int scale_w = cfg->width * cfg->scale;
      int scale_h = cfg->height * cfg->scale;

      if (scale_w > new_monitor_w || scale_h > new_monitor_h)
      {
        --cfg->scale;
        break;
      }

      ++cfg->scale;
    }
  }
  else if (cfg->scale < 2)
  {
    cfg->scale = 2;
  }

  game.display = al_create_display(cfg->width * cfg->scale,
    cfg->height * cfg->scale);

  if (!game.display)
  {
    puts("ERROR: Could not create a display window...");
    return 0;
  }

  al_set_window_title(game.display, cfg->title);

  font = al_create_builtin_font();

  game.timer = al_create_timer(1.0 / cfg->framerate);
  game.event_queue = al_create_event_queue();

  maincfg = cfg;
  set_bg_color(BG_COLOR_DEFAULT);

  ALLEGRO_TRANSFORM trans;
  al_identity_transform(&trans);
  al_scale_transform(&trans, cfg->scale, cfg->scale);
  al_use_transform(&trans);

  game.initialized = TRUE;

  return 1;
}

void game_run(struct State *first)
{
  int redraw = FALSE;

  if (game.is_running)
  {
    puts("WARNING: Calling game_run() more than once");
    return;
  }

  change_state(first);

  // Generate display events
  al_register_event_source(game.event_queue,
    al_get_display_event_source(game.display));

  // Timer events
  al_register_event_source(game.event_queue,
    al_get_timer_event_source(game.timer));

  // Keyboard events
  al_register_event_source(game.event_queue, al_get_keyboard_event_source());

  al_start_timer(game.timer);
  game.is_running = TRUE;

  // Main game loop
  while (game.is_running)
  {
    ALLEGRO_EVENT event;
    al_wait_for_event(game.event_queue, &event);

    // Event processing
    game.states[current_state]->_events(&event);

    // If the close button was pressed...
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      game.is_running = FALSE;
      break;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      keys[event.keyboard.keycode] = TRUE;

      // Escape key will end the game
      if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
      {
        game.is_running = FALSE;
        break;
      }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
      keys[event.keyboard.keycode] = FALSE;
    }
    else if (event.type == ALLEGRO_EVENT_TIMER)
    {
      game.states[current_state]->_update();
      redraw = TRUE;
    }

    if (redraw && al_event_queue_is_empty(game.event_queue))
    {
      redraw = FALSE;

      al_set_target_backbuffer(game.display);

      al_clear_to_color(game.bg_color);

      game.states[current_state]->_draw();

      al_flip_display();
    }
  }

  while (current_state >= 0)
  {
    game.states[current_state--]->_end(TRUE);
  }

  al_destroy_display(game.display);
  al_destroy_timer(game.timer);
  al_destroy_event_queue(game.event_queue);
  al_destroy_font(font);
}

void change_state(struct State *s)
{
  if (game.states[current_state] != NULL)
  {
    game.states[current_state]->_end(FALSE);
  }

  game.states[current_state] = s;
}

void push_state(struct State* s)
{
  if (current_state < (MAX_STATES - 1))
  {
    if (game.states[current_state] != NULL)
    {
      game.states[current_state]->_pause();
    }

    game.states[++current_state] = s;
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
    game.states[current_state]->_end(FALSE);
    game.states[--current_state]->_resume();
  }
  else
  {
    puts("WARNING: Can't remove any more states (current_state = 0)");
  }
}

void game_over(void)
{
  game.is_running = FALSE;
}

void set_bg_color(ALLEGRO_COLOR c)
{
  game.bg_color = c;
}
