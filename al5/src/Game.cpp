#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include "Game.h"
#include "State.h"

ALLEGRO_FONT* font;
bool keys[ALLEGRO_KEY_MAX];

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
  bool redraw;
  bool is_running;
  int width, height;
  std::stack<State_Object*> states;
};

Game::Game_Internal* Game::intern = 0;

bool Game::Init(const char* title, int width, int height, int rate,
                bool want_fs, bool want_audio, bool want_bb)
{
  if (!intern)
  {
    intern = new Game_Internal();
  }
  else
  {
    std::cout << "WARNING: Calling Game::Init() more than once" << std::endl;
    return true;
  }

  intern->buffer = 0;
  intern->redraw = false;
  intern->is_running = false;

  al_init();
  al_install_keyboard();
  al_install_mouse();
  al_init_image_addon();

  if (want_audio)
  {
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
  }

  al_init_font_addon();
  al_init_primitives_addon();

  if (want_fs)
  {
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
  }

  intern->display = al_create_display(width, height);

  if (!intern->display)
  {
    std::cout << "ERROR: Could not create a display" << std::endl;
    delete intern;
    return false;
  }

  aspect_ratio_transform(intern->display, width, height);

  al_set_window_title(intern->display, title);

  al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);

  if (want_bb)
  {
    intern->buffer = al_create_bitmap(width, height);

    if (!intern->buffer)
    {
      std::cout << "ERROR: Could not create a buffer" << std::endl;
      delete intern;
      return false;
    }

    al_set_new_bitmap_flags(0);
  }

  intern->timer = al_create_timer(1.0 / rate);
  intern->event_queue = al_create_event_queue();
  intern->width = width;
  intern->height = height;

  font = al_create_builtin_font();

  Set_BG_Color(BG_COLOR_DEFAULT);

  srand(time(0));

  return true;
}

void Game::Run(State_Object* start_state)
{
  if (intern->is_running)
  {
    std::cout << "WARNING: Calling Game::Run() more than once" << std::endl;

    if (start_state)
    {
      delete start_state;
    }

    return;
  }

  al_register_event_source(intern->event_queue,
                           al_get_display_event_source(intern->display));
  al_register_event_source(intern->event_queue,
                           al_get_keyboard_event_source());
  al_register_event_source(intern->event_queue,
                           al_get_timer_event_source(intern->timer));

  al_start_timer(intern->timer);

  Change_State(start_state);

  intern->is_running = true;

  while (intern->is_running)
  {
    ALLEGRO_EVENT event;
    al_wait_for_event(intern->event_queue, &event);

    intern->states.top()->Events(event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      intern->is_running = false;
      break;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      keys[event.keyboard.keycode] = true;

      if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
      {
        intern->is_running = false;
        break;
      }

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
          al_toggle_display_flag(intern->display,
                                 ALLEGRO_FULLSCREEN_WINDOW, true);
        }

        aspect_ratio_transform(intern->display, intern->width, intern->height);

        al_start_timer(intern->timer);
      }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
      keys[event.keyboard.keycode] = false;
    }
    else if (event.type == ALLEGRO_EVENT_TIMER)
    {
      intern->states.top()->Update();
      intern->redraw = true;
    }

    if (intern->redraw && al_event_queue_is_empty(intern->event_queue))
    {
      intern->redraw = false;

      if (intern->buffer)
      {
        al_set_target_bitmap(intern->buffer);
      }
      else
      {
        al_set_target_backbuffer(intern->display);
      }

      al_clear_to_color(intern->bg_color);

      intern->states.top()->Draw();

      if (intern->buffer)
      {
        al_set_target_backbuffer(intern->display);
        al_clear_to_color(C_BLACK);
        al_draw_bitmap(intern->buffer, 0, 0, 0);
      }

      al_flip_display();
    }
  }

  while (!intern->states.empty())
  {
    delete intern->states.top();
    intern->states.pop();
  }

  al_destroy_display(intern->display);
  al_destroy_bitmap(intern->buffer);
  al_destroy_timer(intern->timer);
  al_destroy_event_queue(intern->event_queue);
  al_destroy_font(font);
}

void Game::Game_Over()
{
  intern->is_running = false;
}

void Game::Set_BG_Color(ALLEGRO_COLOR color)
{
  intern->bg_color = color;
}

void Game::Get_Internal_Res(int& w, int& h)
{
  w = intern->width;
  h = intern->height;
}

void Game::Change_State(State_Object* state)
{
  if (!intern->states.empty())
  {
    delete intern->states.top();
    intern->states.pop();
  }

  intern->states.push(state);
}

void Game::Push_State(State_Object* state)
{
  if (!intern->states.empty())
  {
    intern->states.top()->Pause();
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
    intern->states.top()->Resume();
  }
  else
  {
    intern->is_running = false;
  }
}
