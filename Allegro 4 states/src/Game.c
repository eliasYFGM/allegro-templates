#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>

#include "Game.h"
#include "State.h"


// 0 enables auto-scaling. Any higher value is a fixed screen scale.
#define DEFAULT_SCALE         0

// This is used to simulate a slightly lower monitor resolution when
// auto-scaling is enabled.
#define SCREEN_MULTIPLIER     0.9



// Global(s)
struct Game_Conf *mainconf;



// Internal game engine variables
static struct
{  BITMAP *buffer;
   int initialized;
   int is_running;
   int bg_color;
   int can_change_state;
   int scale;

   // Current running state
   struct State *state;

   // Dynamic array of initialized (loaded) states
   struct State **loaded_states;
   int loaded_count;
} core;



// Game timer
static volatile int ticks;

static void Increment_Ticks(void)
{  ++ticks;
}
END_OF_FUNCTION(Increment_Ticks);



// FPS timer
static volatile int fps, frame_counter;

static void Update_FPS(void)
{  fps = frame_counter;
   frame_counter = 0;
}
END_OF_FUNCTION(Update_FPS);



static void Close_Button_Handler(void)
{  core.is_running = FALSE;
}
END_OF_FUNCTION(Close_Button_Handler);



// Repaint the screen appropriately
static void Update_Screen(void)
{  acquire_screen();

   if (core.scale > 1)
   {  stretch_blit(core.buffer, screen, 0, 0, GAME_W, GAME_H, 0, 0
      , SCREEN_W, SCREEN_H);
   }
   else
   {  blit(core.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
   }

   release_screen();

   ++frame_counter;
}



// Main game initialization
int Game_Init(struct Game_Conf *conf)
{  if (core.initialized)
   {  return 1;
   }

   allegro_init();
   install_keyboard();
   install_mouse();
   install_timer();

   if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
   {  puts("game_init:\n"
           "Failed to initialize audio");
   }

   set_color_depth(conf->depth);

   // Do the auto-scaling if desired
#ifdef ALLEGRO_DOS
   core.scale = 1;
   if (set_gfx_mode(GFX_AUTODETECT, conf->width, conf->height, 0, 0))
#else
   core.scale = DEFAULT_SCALE;

   if (!conf->fullscreen)
   {  if (core.scale <= 0)
      {  int w, h;

         get_desktop_resolution(&w, &h);

         w *= SCREEN_MULTIPLIER;
         h *= SCREEN_MULTIPLIER;

         // Keep scaling until a suitable scale factor is found
         while (1)
         {  int scale_w = conf->width * core.scale;
            int scale_h = conf->height * core.scale;

            if (scale_w > w || scale_h > h)
            {  --core.scale;
               break;
            }

            ++core.scale;
         }

         if (!core.scale)
         {  core.scale = 1;
         }
      }
   }
   else
   {  core.scale = 1;
   }

   if (set_gfx_mode(conf->fullscreen ?
                    GFX_AUTODETECT : GFX_AUTODETECT_WINDOWED
       , conf->width * core.scale, conf->height * core.scale, 0, 0))
#endif // ALLEGRO_DOS

   {  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      printf("game_init:\n"
             "Failed to create a window\n%s", allegro_error);
      return 0;
   }

#ifndef ALLEGRO_DOS
   set_window_title(conf->title);
   set_close_button_callback(Close_Button_Handler);
#endif // ALLEGRO_DOS

   core.buffer = create_bitmap(conf->width, conf->height);
   Set_BG_Color(makecol(192, 192, 192));
   mainconf = conf;
   srand(time(NULL));

   core.can_change_state = TRUE;
   core.initialized = TRUE;

   return 1;
}



// Setup timers and game loop
void Game_Run(struct State *s)
{  int redraw = FALSE;

   if (core.is_running)
   {  return;
   }

   if (!Change_State(s, NULL))
   {  destroy_bitmap(core.buffer);
      return;
   }

   // Main game timer
   LOCK_VARIABLE(ticks);
   LOCK_FUNCTION(Increment_Ticks);
   install_int_ex(Increment_Ticks, BPS_TO_TIMER(mainconf->framerate));

   // FPS timer
   LOCK_VARIABLE(fps);
   LOCK_VARIABLE(frame_counter);
   LOCK_FUNCTION(Update_FPS);
   install_int(Update_FPS, 1000);

   core.is_running = TRUE;

   // Main loop
   while (core.is_running)
   {  while (ticks > 0)
      {  --ticks;

         if (key[KEY_ALT] && key[KEY_ESC])
         {  Game_Over();
            break;
         }

         core.state->On_Update();
         redraw = TRUE;
      }

      if (core.is_running && redraw)
      {  redraw = FALSE;
         clear_to_color(core.buffer, core.bg_color);
         core.state->On_Draw(core.buffer, fps);
         Update_Screen();
      }

      // Yield CPU
      rest(0);
   }

   core.can_change_state = FALSE;

   while (core.loaded_count > 0)
   {  core.loaded_states[--core.loaded_count]->On_Unload();
   }

   free(core.loaded_states);
   destroy_bitmap(core.buffer);
}



int Change_State(struct State *s, void *param)
{  if (!core.can_change_state)
   {  puts("change_state:\n"
           "This function can only be used within state_update() and "
           "state_draw().");
      ticks = 0;
      return TRUE;
   }

   core.can_change_state = FALSE;

   if (!s->loaded)
   {  void *ptr = realloc(core.loaded_states
                  , sizeof *core.loaded_states * core.loaded_count + 1);

      if (ptr != NULL)
      {  if (!s->On_Load(param))
         {  printf("change_state:\n"
                   "\"%s\" state returned 0.\n", s->name);
            core.can_change_state = TRUE;
            return FALSE;
         }

         s->loaded = TRUE;
         core.loaded_states = ptr;
         core.loaded_states[core.loaded_count++] = s;
      }
      else
      {  puts("change_state:\n"
              "realloc() error - Not enough memory?");
         core.can_change_state = TRUE;
         return FALSE;
      }
   }

   s->On_Enter(param);

   if (core.state != NULL)
   {  core.state->On_Exit();
   }

   core.state = s;
   core.can_change_state = TRUE;

   // Reset tick counter
   ticks = 0;

   return TRUE;
}



void Game_Over(void)
{  core.is_running = FALSE;
}



void Set_BG_Color(int c)
{  core.bg_color = c;
}



int Make_Transparent(void)
{  if (mainconf->depth != 8)
   {  return makecol(255, 0, 255);
   }

   return 0;
}
