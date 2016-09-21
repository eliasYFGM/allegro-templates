#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "core.h"
#include "state.h"

#define SCREEN_RES_OVERRIDE   0.1
// Used to simulate a slightly lower screen resolution
// E.g. without the panels and stuff

// Globals
struct Game_Config* default_config;
ALLEGRO_FONT* font;
int keys[ALLEGRO_KEY_MAX];

// The state that is currently updating
static int current_state;

static struct // Game data
{
  ALLEGRO_DISPLAY* display;
  ALLEGRO_TIMER* timer;
  ALLEGRO_EVENT_QUEUE* event_queue;
  ALLEGRO_COLOR bg_color;
  int initialized;
  int is_running;
  struct State* states[MAX_STATES];
}
game;

int game_init(struct Game_Config* config)
{
  if (game.initialized)
  {
    puts("WARNING: Game already initialized");
    return 1;
  }

  // Initialize Allegro and stuff
  al_init();

  if (!al_install_keyboard())
  {
    puts("ERROR: Could not initialize the keyboard...");
    return 0;
  }

  if (!al_install_mouse())
  {
    puts("ERROR: Could not initialize the mouse...");
    return 0;
  }

  if (config->audio)
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

  // Find how much the game will be scaled when config->scale <= 0
  if (config->scale <= 0)
  {
    ALLEGRO_MONITOR_INFO info;
    al_get_monitor_info(0, &info);

    int monitor_w = info.x2 - info.x1;
    int monitor_h = info.y2 - info.y1;

    float new_monitor_w = (monitor_w - (monitor_w * SCREEN_RES_OVERRIDE));
    float new_monitor_h = (monitor_h - (monitor_h * SCREEN_RES_OVERRIDE));

    config->scale = 2;

    // Keep scaling until a suitable scale factor is found
    while (1)
    {
      int scale_w = config->width * config->scale;
      int scale_h = config->height * config->scale;

      if (scale_w > new_monitor_w || scale_h > new_monitor_h)
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

  game.display = al_create_display(config->width * config->scale,
                                   config->height * config->scale);

  if (!game.display)
  {
    puts("ERROR: Could not create a display window...");
    return 0;
  }

  al_set_window_title(game.display, config->title);

  font = al_create_builtin_font();

  game.timer = al_create_timer(1.0 / config->framerate);
  game.event_queue = al_create_event_queue();

  default_config = config;
  set_bg_color(al_map_rgb(192, 192, 192));

  ALLEGRO_TRANSFORM trans;
  al_identity_transform(&trans);
  al_scale_transform(&trans, config->scale, config->scale);
  al_use_transform(&trans);

  game.initialized = TRUE;

  return 1;
}

void game_run()
{
  int redraw = FALSE;

  // Generate display events
  al_register_event_source(game.event_queue,
    al_get_display_event_source(game.display));

  // Timer events
  al_register_event_source(game.event_queue,
    al_get_timer_event_source(game.timer));

  // Keyboard events
  al_register_event_source(game.event_queue, al_get_keyboard_event_source());

  // Mouse events
  al_register_event_source(game.event_queue, al_get_mouse_event_source());

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

  int i;
  for (i=0; i<MAX_STATES; ++i)
  {
    if (game.states[i] != NULL)
    {
      game.states[i]->_end(TRUE);
    }
  }

  al_destroy_display(game.display);
  al_destroy_timer(game.timer);
  al_destroy_event_queue(game.event_queue);
  al_destroy_font(font);
}

void game_over()
{
  game.is_running = FALSE;
}

void set_bg_color(ALLEGRO_COLOR color)
{
  game.bg_color = color;
}

void change_state(struct State* state, long param)
{
  if (game.states[current_state] != NULL)
  {
    game.states[current_state]->_end(FALSE);
  }

  game.states[current_state] = state;
  game.states[current_state]->_init(param);
}

void push_state(struct State* state, long param)
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
