#include <iostream>
#include <stack>
#include <cstdlib>
#include <ctime>
#include "Game.h"
#include "State.h"

static volatile unsigned int ticks = 0;

static void ticker()
{
  ++ticks;
}
END_OF_FUNCTION(ticker)

volatile int fps = 0;
static volatile int fps_counter = 0;

static void update_fps()
{
  fps = fps_counter;
  fps_counter = 0;
}
END_OF_FUNCTION(update_fps)

static volatile bool is_running = false;

static void close_button_handler()
{
  is_running = false;
}
END_OF_FUNCTION(close_button_handler)

struct Game::Game_Internal
{
  BITMAP* buffer;
  bool need_redraw;
  int bg_color;
  int framerate;
  std::stack<State_Object*> states;
};

Game::Game_Internal* Game::intern = 0;

bool Game::Init(const char* title, int width, int height, int rate, int depth,
                bool want_fs, bool want_audio)
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

  intern->need_redraw = false;

  allegro_init();
  install_keyboard();
  install_mouse();
  install_timer();

  if (want_audio)
  {
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
    {
      std::cout << "WARNING: Could not initialize audio" << std::endl;
    }
  }

  set_color_depth(depth);

  if (set_gfx_mode(
    want_fs ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
    width, height, 0, 0))
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("%s\n", allegro_error);
    return false;
  }

  set_window_title(title);

  intern->buffer = create_bitmap(SCREEN_W, SCREEN_H);
  intern->framerate = rate;

  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);

  Set_BG_Color(DEFAULT_BG_COLOR);

  srand(time(0));

  return true;
}

void Game::Run()
{
  if (is_running)
  {
    std::cout << "WARNING: Calling Game::Run() more than once" << std::endl;
    return;
  }

  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(intern->framerate));

  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(fps_counter);
  LOCK_FUNCTION(update_fps);
  install_int(update_fps, 1000);

  is_running = true;

  while (is_running)
  {
    if (ticks > 0)
    {
      while (ticks > 0)
      {
        --ticks;

        if (key[KEY_ESC])
        {
          Game_Over();
          break;
        }

        if (is_running)
        {
          intern->states.top()->Update();
          intern->need_redraw = true;
        }
      }

      if (intern->need_redraw && is_running)
      {
        intern->need_redraw = false;
        clear_to_color(intern->buffer, intern->bg_color);

        intern->states.top()->Draw(intern->buffer);

        show_mouse(intern->buffer);
        blit(intern->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        show_mouse(0);

        ++fps_counter;
      }
    }
    else
    {
      rest(1);
    }
  }

  while (!intern->states.empty())
  {
    delete intern->states.top();
    intern->states.pop();
  }

  destroy_bitmap(intern->buffer);
}

void Game::Game_Over()
{
  is_running = false;
}

void Game::Set_BG_Color(int color)
{
  intern->bg_color = color;
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
    is_running = false;
  }
}
