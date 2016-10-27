#include <iostream>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <allegro.h>
#include "Game_Engine.h"
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

#ifndef ALLEGRO_DOS
static void close_button_handler()
{
  is_running = false;
}
END_OF_FUNCTION(close_button_handler)
#endif

struct Game_Engine::Game_Internal
{
  BITMAP* buffer;
  bool initialized;
  bool need_redraw;
  int bg_color;
  int framerate;
  std::stack<State*> states;
};

Game_Engine::Game_Internal* Game_Engine::pimpl;

int Game_Engine::argc;
char** Game_Engine::argv;

Game_Engine::Game_Engine()
{
  if (!pimpl)
  {
    pimpl = new Game_Internal();
    pimpl->initialized = false;
    pimpl->need_redraw = false;
  }
}

bool Game_Engine::Init(int _argc, char** _argv, const char* title, int width,
                       int height, int rate, int depth, bool want_fs,
                       bool want_audio)
{
  if (pimpl->initialized)
  {
    std::cout << "WARNING: Calling Game_Engine::Init() more than once\n";
    return true;
  }

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

#ifdef ALLEGRO_DOS
  if (set_gfx_mode(GFX_AUTODETECT, width, height, 0, 0))
#else
  if (set_gfx_mode(
    want_fs ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
    width, height, 0, 0))
#endif
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("%s\n", allegro_error);
    return false;
  }

  pimpl->buffer = create_bitmap(SCREEN_W, SCREEN_H);
  pimpl->framerate = rate;

#ifndef ALLEGRO_DOS
  set_window_title(title);

  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);
#endif

  Set_BG_Color(BG_COLOR_DEFAULT);

  argc = _argc;
  argv = _argv;

  srand(time(0));

  pimpl->initialized = true;

  return true;
}

void Game_Engine::Run(State* start_state)
{
  if (!pimpl->initialized)
  {
    std::cout << "ERROR: Game_Engine not yet initialized\n";
    return;
  }

  if (is_running)
  {
    std::cout << "WARNING: Calling Game_Engine::Run() more than once\n";

    if (start_state)
    {
      delete start_state;
    }

    return;
  }

  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(pimpl->framerate));

  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(fps_counter);
  LOCK_FUNCTION(update_fps);
  install_int(update_fps, 1000);

  Change_State(start_state);

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
          pimpl->states.top()->Update();
          pimpl->need_redraw = true;
        }
      }

      if (pimpl->need_redraw && is_running)
      {
        pimpl->need_redraw = false;
        clear_to_color(pimpl->buffer, pimpl->bg_color);

        pimpl->states.top()->Draw(pimpl->buffer);

        show_mouse(pimpl->buffer);
        blit(pimpl->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        show_mouse(0);

        ++fps_counter;
      }
    }
#ifndef ALLEGRO_DOS
    else
    {
      rest(1);
    }
#endif
  }

  while (!pimpl->states.empty())
  {
    delete pimpl->states.top();
    pimpl->states.pop();
  }

  destroy_bitmap(pimpl->buffer);

  delete pimpl;
}

void Game_Engine::Game_Over()
{
  is_running = false;
}

void Game_Engine::Set_BG_Color(int color)
{
  pimpl->bg_color = color;
}

void Game_Engine::Change_State(State* state)
{
  if (!pimpl->states.empty())
  {
    delete pimpl->states.top();
    pimpl->states.pop();
  }

  pimpl->states.push(state);
}

void Game_Engine::Push_State(State* state)
{
  if (!pimpl->states.empty())
  {
    pimpl->states.top()->Pause();
  }

  pimpl->states.push(state);
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
    is_running = false;
  }
}
