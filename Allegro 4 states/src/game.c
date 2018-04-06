#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#include "game.h"
#include "state.h"


// 0 enables auto-scaling. Any higher value is a fixed screen scale.
#define DEFAULT_SCALE         0

// This is used to simulate a slightly lower monitor resolution when
// auto-scaling is enabled.
#define SCREEN_RES_OVERRIDE   0.1

// Globals
volatile int fps;
struct Game_Conf *mainconf;



// Internal variables
static struct
{  BITMAP *buffer;
   int game_active;
   int game_initialized;
   int bg_color;
   int can_change;
   int scale;
   volatile int frame_counter;
   volatile int ticks;

   // Actual running state
   struct State *state;

   // Dynamic array of initialized (loaded) states
   struct State **loaded_states;
   int loaded_count;
} data;



static void ticker(void)
{
   ++data.ticks;
}
END_OF_FUNCTION(ticker);



static void update_fps(void)
{
   fps = data.frame_counter;
   data.frame_counter = 0;
}
END_OF_FUNCTION(update_fps);



#ifndef ALLEGRO_DOS
static void close_button_handler(void)
{
   data.game_active = FALSE;
}
END_OF_FUNCTION(close_button_handler);
#endif // ALLEGRO_DOS



// Repaint the screen appropriately
static void update_screen(void)
{
#ifdef ALLEGRO_DOS
   blit(data.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
#else
   if (data.scale > 1)
   {  stretch_blit(data.buffer, screen, 0, 0, GAME_W, GAME_H, 0, 0
                   , SCREEN_W, SCREEN_H);
   }
   else
   {  blit(data.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
   }
#endif // ALLEGRO_DOS

   ++data.frame_counter;
}



// Main game initialization
int game_init(struct Game_Conf *conf)
{
   if (data.game_initialized)
   {  return 1;
   }

   allegro_init();
   install_keyboard();
   install_mouse();
   install_timer();

   if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
   {  allegro_message("engine_init():\n"
                      "Failed to initialize audio");
   }

   set_color_depth(conf->depth);

#ifdef ALLEGRO_DOS
   if (set_gfx_mode(GFX_AUTODETECT, conf->width, conf->height, 0, 0))
#else
   if (!conf->fullscreen)
   {  if (DEFAULT_SCALE <= 0)
      {  int w, h, new_w, new_h;

         get_desktop_resolution(&w, &h);

         new_w = w - (w * SCREEN_RES_OVERRIDE);
         new_h = h - (h * SCREEN_RES_OVERRIDE);

         // Keep scaling until a suitable scale factor is found
         while (1)
         {  int scale_w = conf->width * data.scale;
            int scale_h = conf->height * data.scale;

            if (scale_w > new_w || scale_h > new_h)
            {  --data.scale;
               break;
            }

            ++data.scale;
         }

         if (!data.scale)
         {  data.scale = 1;
         }
      }
      else
      {  data.scale = DEFAULT_SCALE;
      }
   }

   if (set_gfx_mode(conf->fullscreen ? GFX_AUTODETECT
                    : GFX_AUTODETECT_WINDOWED, conf->width * data.scale
                    , conf->height * data.scale, 0, 0))
#endif // ALLEGRO_DOS
   {  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("engine_init():\n"
                      "Failed to create a window\n%s", allegro_error);
      return 0;
   }

#ifndef ALLEGRO_DOS
   set_window_title(conf->title);
   set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

   data.buffer = create_bitmap(conf->width, conf->height);

   set_bg_color(makecol(192, 192, 192));

   mainconf = conf;

   srand(time(NULL));

   data.can_change = TRUE;
   data.game_initialized = TRUE;

   return 1;
}



// Setup timers and game loop
void game_run(struct State *s)
{
   int redraw = FALSE;

   if (data.game_active)
   {  return;
   }

   if (!change_state(s, NULL))
   {  allegro_message("game_run():\n"
                      "Error while initializing the state");
      destroy_bitmap(data.buffer);
      return;
   }

   // Main game timer
   LOCK_VARIABLE(data.ticks);
   LOCK_FUNCTION(ticker);
   install_int_ex(ticker, BPS_TO_TIMER(mainconf->framerate));

   // FPS timer
   LOCK_VARIABLE(fps);
   LOCK_VARIABLE(data.frame_counter);
   LOCK_FUNCTION(update_fps);
   install_int(update_fps, 1000);

   data.game_active = TRUE;

   // Main loop
   while (data.game_active)
   {  while (data.ticks > 0)
      {  --data.ticks;

         if (key[KEY_ALT] && key[KEY_F4])
         {  game_over();
            break;
         }

         data.state->update();
         redraw = TRUE;
      }

      if (data.game_active && redraw)
      {  redraw = FALSE;
         clear_to_color(data.buffer, data.bg_color);
         data.state->draw(data.buffer);
         update_screen();
      }

      // Yield CPU
      rest(0);
   }

   data.can_change = FALSE;

   while (data.loaded_count > 0)
   {  data.loaded_states[--data.loaded_count]->unload();
   }

   free(data.loaded_states);
   destroy_bitmap(data.buffer);
}



int change_state(struct State *s, void *param)
{
   if (!data.can_change)
   {  allegro_message("change_state():\n"
                      "This function can only be used within state_update() "
                      "and state_draw().");
      data.ticks = 0;
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
      {  allegro_message("change_state():\n"
                         "realloc error");
         return FALSE;
      }
   }

   s->enter(param);

   if (data.state != NULL)
   {  data.state->exit();
   }

   data.state = s;
   data.can_change = TRUE;

   // Reset tick counter
   data.ticks = 0;

   return TRUE;
}



void game_over(void)
{
   data.game_active = FALSE;
}



void set_bg_color(int c)
{
   data.bg_color = c;
}
