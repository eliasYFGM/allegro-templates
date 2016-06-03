#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define C_BLACK  al_map_rgb(0, 0, 0)
#define C_WHITE  al_map_rgb(255, 255, 255)

// Internal resolution
#define SCREEN_W  640
#define SCREEN_H  480

// Window title
#define AL_WINDOW_TITLE   "Allegro application"

// Framerate
#define AL_REFRESH_RATE   40

// Whether to use full-screen
static bool AL_USE_FULLSCREEN = false;

// Whether to show the mouse cursor
static bool AL_SHOW_CURSOR = true;

// Whether to use audio
static bool AL_ENABLE_AUDIO = false;

// Updates the aspect ratio when going fullscreen
static void aspect_ratio_transform(ALLEGRO_DISPLAY* display)
{
  int window_w = al_get_display_width(display);
  int window_h = al_get_display_height(display);

  float sw = (window_w / (float)(SCREEN_W));
  float sh = (window_h / (float)(SCREEN_H));
  float scale = std::min(sw, sh);

  float scale_w = (SCREEN_W * scale);
  float scale_h = (SCREEN_H * scale);
  int scale_x_pos = (window_w - scale_w) / 2;
  int scale_y_pos = (window_h - scale_h) / 2;

  ALLEGRO_TRANSFORM trans;
  al_identity_transform(&trans);
  al_build_transform(&trans, scale_x_pos, scale_y_pos, scale, scale, 0);
  al_use_transform(&trans);
}

static struct // Game data
{
  ALLEGRO_DISPLAY*    display;
  ALLEGRO_BITMAP*     buffer;
  ALLEGRO_TIMER*      timer;
  ALLEGRO_EVENT_QUEUE*  event_queue;
  ALLEGRO_FONT*       font;
  bool is_running;
}
game =
{
  NULL, NULL, NULL, NULL, NULL,
  0
};

// This is called already after shutdown_game()
static void unload_resources()
{
  al_destroy_display(game.display);
  al_destroy_bitmap(game.buffer);
  al_destroy_timer(game.timer);
  al_destroy_event_queue(game.event_queue);
  al_destroy_font(game.font);
}

//---------------------------------------------------
// Game functions
//---------------------------------------------------
bool init_game()
{
  // Always return 'true' on success
  return true;
}

void process_events(ALLEGRO_EVENT& event)
{
  // TODO: event processing
}

void update_game()
{
  // TODO: game logic
}

void render_game()
{
  // TODO: game rendering
}

void shutdown_game()
{
  // TODO: free resources
}

//---------------------------------------------------
// The main procedure
//---------------------------------------------------
int main(int argc, char **argv)
{
  // Init Allegro and stuff
  al_init();

  if (!al_install_keyboard())
  {
    std::cout << "Could not initialize the keyboard..." << std::endl;
    return -1;
  }

  if (!al_install_mouse())
  {
    std::cout << "Could not initialize the mouse..." << std::endl;
    return -1;
  }

  if (AL_ENABLE_AUDIO)
  {
    if (!al_install_audio())
    {
      std::cout << "Could not initialize audio..." << std::endl;
      return -1;
    }

    if (!al_init_acodec_addon())
    {
      std::cout << "Could not initialize acodec addon..." << std::endl;
      return -1;
    }
  }

  // Addons
  if (!al_init_image_addon())
  {
    std::cout << "Could not initialize image addon..." << std::endl;
    return -1;
  }

  al_init_font_addon();

  if (AL_USE_FULLSCREEN)
  {
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
  }

  game.display = al_create_display(SCREEN_W, SCREEN_H);
  al_set_window_title(game.display, AL_WINDOW_TITLE);

  // Use linear filtering for scaling game screen
  al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);

  game.buffer = al_create_bitmap(SCREEN_W, SCREEN_H);
  game.timer = al_create_timer(1.0 / AL_REFRESH_RATE);
  game.event_queue = al_create_event_queue();
  game.font = al_create_builtin_font();

  aspect_ratio_transform(game.display);

  // Setup game
  if (!init_game())
  {
    unload_resources();
    return -1;
  }

  if (!AL_SHOW_CURSOR)
  {
    al_hide_mouse_cursor(game.display);
  }

  al_register_event_source(game.event_queue, al_get_display_event_source(game.display));
  al_register_event_source(game.event_queue, al_get_keyboard_event_source());
  al_register_event_source(game.event_queue, al_get_mouse_event_source());
  al_register_event_source(game.event_queue, al_get_timer_event_source(game.timer));

  al_start_timer(game.timer);

  game.is_running = true;

  // Game loop
  while (game.is_running)
  {
    bool redraw = false;

    ALLEGRO_EVENT event;
    al_wait_for_event(game.event_queue, &event);

    process_events(event);

    // If the close button was pressed...
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      game.is_running = false;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      // Escape key will end the game
      if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
      {
        game.is_running = false;
      }

      // F4 key will change between screen modes
      if (event.keyboard.keycode == ALLEGRO_KEY_F4)
      {
        if (AL_USE_FULLSCREEN)
        {
          AL_USE_FULLSCREEN = false;
        }
        else
        {
          AL_USE_FULLSCREEN = true;
        }

        al_toggle_display_flag(game.display, ALLEGRO_FULLSCREEN_WINDOW, AL_USE_FULLSCREEN);
        aspect_ratio_transform(game.display);
      }
    }
    else if (event.type == ALLEGRO_EVENT_TIMER)
    {
      update_game();
      redraw = true;
    }

    if (redraw && al_event_queue_is_empty(game.event_queue))
    {
      redraw = false;

      al_set_target_bitmap(game.buffer);

      al_clear_to_color(al_map_rgb(192, 192, 192));

      render_game();

      al_set_target_backbuffer(game.display);

      al_draw_bitmap(game.buffer, 0, 0, 0);

      al_flip_display();
    }
  }

  shutdown_game();

  unload_resources();

  return 0;
}
