#include <iostream>
#include <stack>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include "Game_Engine.h"
#include "State.h"

bool engine_active;
bool key[ALLEGRO_KEY_MAX];
ALLEGRO_FONT *font;

static void aspect_ratio_transform(ALLEGRO_DISPLAY *display, int w, int h)
{
  int window_w = al_get_display_width(display);
  int window_h = al_get_display_height(display);

  float sw = (window_w / (float) w);
  float sh = (window_h / (float) h);
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

struct Game_Engine::Impl
{
  ALLEGRO_DISPLAY *display;
  ALLEGRO_BITMAP *buffer;
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_COLOR bg_color;
  bool initialized;
  bool redraw;
  std::stack<State*> states;
};

Game_Engine::Game_Engine() : pimpl(new Impl())
{
  pimpl->initialized = false;
  pimpl->buffer = 0;
  pimpl->redraw = false;
}

Game_Engine::~Game_Engine()
{
}

bool Game_Engine::Init(int argc, char** argv, const char* title, int w, int h,
                       int rate, bool full, bool audio, bool backbuff)
{
  if (pimpl->initialized)
  {
    std::cout << "WARNING: Calling Game_Engine::Init() more than once\n";
    return true;
  }

  al_init();
  al_install_keyboard();
  al_install_mouse();
  al_init_image_addon();

  if (audio)
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

  if (full)
  {
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
  }

  width = w;
  height = h;

  pimpl->display = al_create_display(w, h);

  if (!pimpl->display)
  {
    std::cout << "ERROR: Could not create a display" << std::endl;
    return false;
  }

  aspect_ratio_transform(pimpl->display, w, h);

  al_set_window_title(pimpl->display, title);

  al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);

  if (backbuff)
  {
    pimpl->buffer = al_create_bitmap(w, h);

    if (!pimpl->buffer)
    {
      std::cout << "ERROR: Could not create a buffer" << std::endl;
      return false;
    }

    al_set_new_bitmap_flags(0);
  }

  pimpl->timer = al_create_timer(1.0 / rate);
  pimpl->event_queue = al_create_event_queue();

  font = al_create_builtin_font();

  Set_BG_Color(BG_COLOR_DEFAULT);

  args.assign(argv, argv + argc);

  pimpl->initialized = true;

  return true;
}

void Game_Engine::Run(State *first)
{
  if (!pimpl->initialized)
  {
    std::cout << "ERROR: Game_Engine not yet initialized" << std::endl;
    return;
  }

  if (engine_active)
  {
    std::cout << "WARNING: Calling Game_Engine::Run() more than once\n";

    if (first)
    {
      delete first;
    }

    return;
  }

  Change_State(first);

  al_register_event_source(pimpl->event_queue,
    al_get_display_event_source(pimpl->display));

  al_register_event_source(pimpl->event_queue,
    al_get_keyboard_event_source());

  al_register_event_source(pimpl->event_queue,
    al_get_timer_event_source(pimpl->timer));

  al_start_timer(pimpl->timer);

  engine_active = true;

  while (engine_active)
  {
    ALLEGRO_EVENT event;
    al_wait_for_event(pimpl->event_queue, &event);

    pimpl->states.top()->Events(this, event);

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      engine_active = false;
      break;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      key[event.keyboard.keycode] = true;

      if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
      {
        engine_active = false;
        break;
      }

      if (event.keyboard.keycode == ALLEGRO_KEY_F4)
      {
        al_stop_timer(pimpl->timer);

        if (al_get_display_flags(pimpl->display) & ALLEGRO_FULLSCREEN_WINDOW)
        {
          al_toggle_display_flag(pimpl->display,
            ALLEGRO_FULLSCREEN_WINDOW, false);
        }
        else
        {
          al_toggle_display_flag(pimpl->display,
            ALLEGRO_FULLSCREEN_WINDOW, true);
        }

        aspect_ratio_transform(pimpl->display, width, height);

        al_start_timer(pimpl->timer);
      }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
      key[event.keyboard.keycode] = false;
    }
    else if (event.type == ALLEGRO_EVENT_TIMER)
    {
      pimpl->states.top()->Update(this);
      pimpl->redraw = true;
    }

    if (pimpl->redraw && al_event_queue_is_empty(pimpl->event_queue))
    {
      pimpl->redraw = false;

      if (pimpl->buffer)
      {
        al_set_target_bitmap(pimpl->buffer);
      }
      else
      {
        al_set_target_backbuffer(pimpl->display);
      }

      al_clear_to_color(pimpl->bg_color);

      pimpl->states.top()->Draw();

      if (pimpl->buffer)
      {
        al_set_target_backbuffer(pimpl->display);
        al_clear_to_color(C_BLACK);
        al_draw_bitmap(pimpl->buffer, 0, 0, 0);
      }

      al_flip_display();
    }
  }

  while (!pimpl->states.empty())
  {
    delete pimpl->states.top();
    pimpl->states.pop();
  }

  al_destroy_display(pimpl->display);
  al_destroy_bitmap(pimpl->buffer);
  al_destroy_timer(pimpl->timer);
  al_destroy_event_queue(pimpl->event_queue);
  al_destroy_font(font);
}

void Game_Engine::Change_State(State *s)
{
  if (!pimpl->states.empty())
  {
    delete pimpl->states.top();
    pimpl->states.pop();
  }

  pimpl->states.push(s);
}

void Game_Engine::Push_State(State *s)
{
  if (!pimpl->states.empty())
  {
    pimpl->states.top()->Pause();
  }

  pimpl->states.push(s);
}

void Game_Engine::Pop_State()
{
  if (!pimpl->states.empty())
  {
    delete pimpl->states.top();
    pimpl->states.pop();
  }

  if (!pimpl->states.empty())
  {
    pimpl->states.top()->Resume();
  }
  else
  {
    engine_active = false;
  }
}

void Game_Engine::Set_BG_Color(ALLEGRO_COLOR c)
{
  pimpl->bg_color = c;
}
