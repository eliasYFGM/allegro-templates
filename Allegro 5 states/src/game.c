#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "game.h"
#include "state.h"

// Globals
ALLEGRO_FONT *font;
struct Game_Conf *mainconf;


// Updates the game screen when going full-screen or windowed
static void fix_game_screen(ALLEGRO_DISPLAY *display)
{
   int window_w = al_get_display_width(display);
   int window_h = al_get_display_height(display);

   float sw = (float) window_w / GAME_W;
   float sh = (float) window_h / GAME_H;
   float scale = (sw < sh ? sw : sh);

   float scale_w = (float) GAME_W * scale;
   float scale_h = (float) GAME_H * scale;
   int scale_x_pos = (window_w - scale_w) / 2;
   int scale_y_pos = (window_h - scale_h) / 2;

   ALLEGRO_TRANSFORM trans;
   al_identity_transform(&trans);
   al_build_transform(&trans, scale_x_pos, scale_y_pos, scale, scale, 0);
   al_use_transform(&trans);
}



static void change_video_mode(ALLEGRO_DISPLAY *display)
{
   int flags = al_get_display_flags(display);

   if (flags & ALLEGRO_FULLSCREEN_WINDOW)
   {  al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 0);
   }
   else
   {  al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, 1);
   }

   fix_game_screen(display);
}



// Engine variables
static struct
{  ALLEGRO_DISPLAY *display;
   ALLEGRO_BITMAP *buffer;
   ALLEGRO_TIMER *timer;
   ALLEGRO_EVENT_QUEUE *event_queue;
   ALLEGRO_COLOR bg_color;
   int game_initialized;
   int game_active;
   int can_change;

   // Actual running state
   struct State *state;

   // Dynamic array of initialized (loaded) states.
   struct State **loaded_states;
   int loaded_count;
} data;



int game_init(struct Game_Conf *conf)
{
   if (data.game_initialized)
   {  return 1;
   }

   // Initialize Allegro and stuff
   al_init();

   if (!al_install_keyboard())
   {  puts("game_init():\n"
           "Failed to initialize the keyboard...");
      return 0;
   }

   if (!al_install_mouse())
   {  puts("game_init():\n"
           "Failed to initialize the mouse...");
      return 0;
   }

   // Add-ons
   if (!al_init_image_addon())
   {  puts("game_init():\n"
           "Failed to initialize image addon...");
      return 0;
   }

   if (!al_init_font_addon())
   {  puts("game_init():\n"
           "Failed to initialize font addon...");
      return 0;
   }

   if (conf->fullscreen)
   {  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
   }

   // Initialize variables...
   data.display = al_create_display(conf->width, conf->height);

   if (!data.display)
   {  puts("game_init():\n"
           "Failed to create a display window...");
      return 0;
   }

   al_set_window_title(data.display, conf->title);

   mainconf = conf;
   fix_game_screen(data.display);

   al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);

   if (conf->buffer)
   {  data.buffer = al_create_bitmap(conf->width, conf->height);
      al_set_new_bitmap_flags(0);
   }

   font = al_create_builtin_font();
   data.timer = al_create_timer(1.0 / conf->framerate);
   data.event_queue = al_create_event_queue();

   set_bg_color(al_map_rgb(192, 192, 192));

   srand(time(NULL));

   data.game_initialized = TRUE;
   data.can_change = TRUE;

   return 1;
}



void game_run(struct State *s)
{
   int redraw = FALSE;

   if (data.game_active)
   {  return;
   }

   if (!change_state(s, NULL))
   {  puts("game_run():\n"
           "State initialization failed");
      return;
   }

   // Default array for key presses
   int *key = calloc(ALLEGRO_KEY_MAX, sizeof *key);

   // Generate display events
   al_register_event_source(data.event_queue
                            , al_get_display_event_source(data.display));

   // Timer events
   al_register_event_source(data.event_queue
                            , al_get_timer_event_source(data.timer));

   // Keyboard events
   al_register_event_source(data.event_queue
                            , al_get_keyboard_event_source());

   // Mouse events
   al_register_event_source(data.event_queue
                            , al_get_mouse_event_source());

   al_start_timer(data.timer);

   data.game_active = TRUE;

   // Main game loop
   while (data.game_active)
   {  ALLEGRO_EVENT event;
      al_wait_for_event(data.event_queue, &event);

      // Event processing
      data.state->events(&event);

      // If the close button was pressed...
      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
      {  data.game_active = FALSE;
         break;
      }
      else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
      {  key[event.keyboard.keycode] = TRUE;

         // F4 key will toggle full-screen
         if (key[ALLEGRO_KEY_F4])
         {  al_stop_timer(data.timer);
            change_video_mode(data.display);
            al_start_timer(data.timer);
         }
      }
      else if (event.type == ALLEGRO_EVENT_KEY_UP)
      {  key[event.keyboard.keycode] = FALSE;
      }
      else if (event.type == ALLEGRO_EVENT_TIMER)
      {  data.state->update(key);
         redraw = TRUE;
      }

      if (redraw && data.game_active
            && al_is_event_queue_empty(data.event_queue))
      {  redraw = FALSE;

         if (mainconf->buffer)
         {  al_set_target_bitmap(data.buffer);
         }
         else
         {  al_set_target_backbuffer(data.display);
         }

         al_clear_to_color(data.bg_color);

         data.state->draw();

         if (mainconf->buffer)
         {  al_set_target_backbuffer(data.display);
            al_clear_to_color(C_BLACK);
            al_draw_bitmap(data.buffer, 0, 0, 0);
         }

         al_flip_display();
      }
   }

   data.can_change = FALSE;

   while (data.loaded_count)
   {  data.loaded_states[--data.loaded_count]->unload();
   }

   free(data.loaded_states);

   al_destroy_display(data.display);
   al_destroy_timer(data.timer);
   al_destroy_event_queue(data.event_queue);
   al_destroy_font(font);

   if (mainconf->buffer)
   {  al_destroy_bitmap(data.buffer);
   }

   free(key);
}



int change_state(struct State *s, void *param)
{
   if (!data.can_change)
   {  puts("change_state():\n"
           "States can only be changed within events(), update() and draw() "
           "functions.");
      return TRUE;
   }

   data.can_change = FALSE;

   if (!s->loaded)
   {  void *ptr = realloc(data.loaded_states
                          , sizeof *data.loaded_states * data.loaded_count + 1);

      if (ptr != NULL)
      {  if (!s->load(param))
         {  return FALSE;
         }

         s->loaded = TRUE;
         data.loaded_states = ptr;
         data.loaded_states[data.loaded_count++] = s;
      }
      else
      {  puts("change_state():\n"
              "realloc() error");
         return FALSE;
      }
   }

   s->enter(param);

   if (data.state != NULL)
   {  data.state->exit();
   }

   data.state = s;
   data.can_change = TRUE;

   al_flush_event_queue(data.event_queue);

   return TRUE;
}



void game_over(void)
{
   data.game_active = FALSE;
}



void set_bg_color(ALLEGRO_COLOR c)
{
   data.bg_color = c;
}
