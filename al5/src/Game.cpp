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

ALLEGRO_FONT* font = 0;

static void aspect_ratio_transform(ALLEGRO_DISPLAY* display, int w, int h)
{
  int window_w = al_get_display_width(display);
  int window_h = al_get_display_height(display);

  float sw = (window_w / (float)(w));
  float sh = (window_h / (float)(h));
  float scale = std::min(sw, sh);

  float scale_w = (w * scale);
  float scale_h = (h * scale);
  int scale_x_pos = (window_w - scale_w) / 2;
  int scale_y_pos = (window_h - scale_h) / 2;

  ALLEGRO_TRANSFORM trans;
  al_identity_transform(&trans);
  al_build_transform(&trans, scale_x_pos, scale_y_pos, scale, scale, 0);
  al_use_transform(&trans);
}

struct Game::Game_Internal
{
  ALLEGRO_DISPLAY* display;
  ALLEGRO_BITMAP* buffer;
  ALLEGRO_TIMER* timer;
  ALLEGRO_EVENT_QUEUE* event_queue;
  ALLEGRO_COLOR bg_color;
  bool need_update;
  bool is_running;
  int width, height;
  std::stack<State*> states;
};

Game::Game()
{
  intern = new Game_Internal();
  intern->need_update = false;
  intern->is_running = false;
}

Game::~Game()
{
  al_destroy_display(intern->display);
  al_destroy_bitmap(intern->buffer);
  al_destroy_timer(intern->timer);
  al_destroy_event_queue(intern->event_queue);
  al_destroy_font(font);

  delete intern;
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

  intern->display = al_create_display(width, height);

  if (!intern->display)
  {
    std::cout << "ERROR: Could not create a display" << std::endl;
    return false;
  }

  aspect_ratio_transform(intern->display, width, height);

  al_set_window_title(intern->display, title);

  al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);

  intern->buffer = al_create_bitmap(width, height);

  if (!intern->buffer)
  {
    std::cout << "ERROR: Could not create a buffer" << std::endl;
    return false;
  }

  intern->timer = al_create_timer(1.0 / rate);
  intern->event_queue = al_create_event_queue();
  intern->width = width;
  intern->height = height;

  font = al_create_builtin_font();

  Set_BG_Color(al_map_rgb(192, 192, 192));

  srand(time(0));

  return true;
}

void Game::Run()
{
  al_register_event_source(intern->event_queue,
                           al_get_display_event_source(intern->display));
  al_register_event_source(intern->event_queue,
                           al_get_keyboard_event_source());
  al_register_event_source(intern->event_queue,
                           al_get_timer_event_source(intern->timer));

  al_start_timer(intern->timer);

  intern->is_running = true;

  while (intern->is_running)
  {
    Handle_Events();
    Update();
    Draw();
  }

  while (!intern->states.empty())
  {
    delete intern->states.top();
    intern->states.pop();
  }
}

void Game::Handle_Events()
{
  ALLEGRO_EVENT event;
  al_wait_for_event(intern->event_queue, &event);

  if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
  {
    intern->is_running = false;
  }

  if (event.type == ALLEGRO_EVENT_KEY_DOWN)
  {
    if (event.keyboard.keycode == ALLEGRO_KEY_F4)
    {
      al_stop_timer(intern->timer);

      if (al_get_display_flags(intern->display) & ALLEGRO_FULLSCREEN_WINDOW)
      {
        al_toggle_display_flag(intern->display,
                               ALLEGRO_FULLSCREEN_WINDOW, false);
      }
      else
      {
        al_toggle_display_flag(intern->display, ALLEGRO_FULLSCREEN_WINDOW,
                               true);
      }

      aspect_ratio_transform(intern->display, intern->width, intern->height);

      al_start_timer(intern->timer);
    }
  }

  if (event.type == ALLEGRO_EVENT_TIMER)
  {
    intern->need_update = true;
  }

  if (intern->is_running)
  {
    intern->states.top()->Handle_Events(this, event);
  }
}

void Game::Update()
{
  if (intern->need_update && intern->is_running)
  {
    intern->need_update = false;
    intern->states.top()->Update(this);
  }
}

void Game::Draw()
{
  if (intern->is_running && al_event_queue_is_empty(intern->event_queue))
  {
    al_set_target_bitmap(intern->buffer);
    al_clear_to_color(intern->bg_color);

    intern->states.top()->Draw(this);

    al_set_target_backbuffer(intern->display);
    al_clear_to_color(C_BLACK);

    al_draw_bitmap(intern->buffer, 0, 0, 0);

    al_flip_display();
  }
}

void Game::Game_Over()
{
  intern->is_running = false;
}

void Game::Set_BG_Color(ALLEGRO_COLOR color)
{
  intern->bg_color = color;
}

void Game::Get_Original_Res(int& w, int& h)
{
  w = intern->width;
  h = intern->height;
}

void Game::Change_State(State* state)
{
  if (!intern->states.empty())
  {
    delete intern->states.top();
    intern->states.pop();
  }

  intern->states.push(state);
}

void Game::Push_State(State* state)
{
  if (!intern->states.empty())
  {
    intern->states.top()->Pause(this);
  }

  intern->states.push(state);
}

void Game::Pop_State()
{
  if (!intern->states.empty())
  {
    delete intern->states.top();
    intern->states.pop();
  }

  if (!intern->states.empty())
  {
    intern->states.top()->Resume(this);
  }
  else
  {
    intern->is_running = false;
  }
}
