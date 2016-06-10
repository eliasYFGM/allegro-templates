#include <iostream>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "Game.h"

int Game::internal_w = 0;
int Game::internal_h = 0;

ALLEGRO_FONT* font = 0;

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

Game::~Game()
{
  al_destroy_display(display);
  al_destroy_bitmap(buffer);
  al_destroy_timer(timer);
  al_destroy_font(font);
  al_destroy_event_queue(event_queue);
}

bool Game::Init(int width, int height, const char* title, bool fullscreen,
  int rate)
{
  al_init();
  al_install_keyboard();
  al_install_mouse();
  al_init_image_addon();

  if (al_install_audio())
  {
    if (!al_init_acodec_addon())
    {
      std::cout << "WARNING: Could not init acodec addon" << std::endl;
    }
  }
  else
  {
    std::cout << "WARNING: Could not init audio" << std::endl;
  }

  al_init_font_addon();
  al_init_primitives_addon();

  if (fullscreen)
  {
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
  }

  display = al_create_display(width, height);

  if (!display)
  {
    std::cout << "ERROR: Could not create a display" << std::endl;
    return false;
  }

  internal_w = width;
  internal_h = height;

  aspect_ratio_transform(display);
  al_set_window_title(display, title);

  al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);

  buffer = al_create_bitmap(width, height);

  if (!buffer)
  {
    std::cout << "ERROR: Could not create a buffer" << std::endl;
    return false;
  }

  timer = al_create_timer(1.0 / rate);
  font = al_create_builtin_font();
  event_queue = al_create_event_queue();

  Set_BG_Color(al_map_rgb(192, 192, 192));

  srand(time(0));

  return true;
}

void Game::Run()
{
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));

  al_start_timer(timer);

  is_running = true;

  while (is_running)
  {
    Handle_Events();
    Update();
    Draw();
  }

  while (!states.empty())
  {
    delete states.top();
    states.pop();
  }
}

void Game::Handle_Events()
{
  ALLEGRO_EVENT event;
  al_wait_for_event(event_queue, &event);

  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
  {
    is_running = false;
  }

  if (event.type == ALLEGRO_EVENT_KEY_DOWN)
  {
    if (event.keyboard.keycode == ALLEGRO_KEY_F4)
    {
      al_stop_timer(timer);

      if (al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW)
      {
        al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
      }
      else
      {
        al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
      }

      aspect_ratio_transform(display);

      al_start_timer(timer);
    }
  }

  if (event.type == ALLEGRO_EVENT_TIMER)
  {
    need_update = true;
  }

  if (is_running)
  {
    states.top()->Handle_Events(this, event);
  }
}

void Game::Update()
{
  if (need_update && is_running)
  {
    need_update = false;
    states.top()->Update(this);
  }
}

void Game::Draw()
{
  if (is_running && al_event_queue_is_empty(event_queue))
  {
    al_set_target_bitmap(buffer);

    al_clear_to_color(bg_color);

    states.top()->Draw(this);

    al_set_target_backbuffer(display);

    al_clear_to_color(C_BLACK);

    al_draw_bitmap(buffer, 0, 0, 0);

    al_flip_display();
  }
}

void Game::Change_State(State* state)
{
  if (!states.empty())
  {
    delete states.top();
    states.pop();
  }

  states.push(state);
}

void Game::Push_State(State* state)
{
  if (!states.empty())
  {
    states.top()->Pause(this);
  }

  states.push(state);
}

void Game::Pop_State()
{
  if (!states.empty())
  {
    delete states.top();
    states.pop();
  }

  if (!states.empty())
  {
    states.top()->Resume(this);
  }
  else
  {
    is_running = false;
  }
}
