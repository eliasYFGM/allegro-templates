#include <iostream>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <allegro.h>
#include "Game_Engine.h"
#include "State.h"

static volatile unsigned int ticks;

static void ticker()
{
  ++ticks;
}
END_OF_FUNCTION(ticker)

volatile int fps;
static volatile int fps_counter;

static void update_fps()
{
  fps = fps_counter;
  fps_counter = 0;
}
END_OF_FUNCTION(update_fps)

volatile bool engine_active;

#ifndef ALLEGRO_DOS
static void close_button_handler()
{
  engine_active = false;
}
END_OF_FUNCTION(close_button_handler)
#endif

struct Game_Engine::Impl
{
  BITMAP *buffer;
  bool initialized;
  bool need_redraw;
  bool mouse;
  bool cursor;
  int bg_color;
  int framerate;
  std::stack<State*> states;
};

Game_Engine::Game_Engine() : pimpl(new Impl())
{
  pimpl->initialized = false;
  pimpl->need_redraw = false;
}

Game_Engine::~Game_Engine()
{
}

bool Game_Engine::Init(int argc, char **argv, const char *title, int width,
                       int height, int rate,  int depth, bool full, bool mouse,
                       bool audio)
{
  if (pimpl->initialized)
  {
    std::cout << "WARNING: Calling Game_Engine::Init() more than once\n";
    return true;
  }

  allegro_init();
  install_keyboard();
  install_timer();

  if (mouse)
  {
    install_mouse();
  }

  if (audio)
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
    full ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
    width, height, 0, 0))
#endif
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("%s\n", allegro_error);
    return false;
  }

  pimpl->buffer = create_bitmap(SCREEN_W, SCREEN_H);
  pimpl->framerate = rate;
  pimpl->mouse = mouse;
  pimpl->cursor = mouse;

#ifndef ALLEGRO_DOS
  set_window_title(title);

  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);
#endif

  Set_BG_Color(BG_COLOR_DEFAULT);

  args.assign(argv, argv + argc);

  srand(time(0));

  pimpl->initialized = true;

  return true;
}

void Game_Engine::Run(State *first)
{
  if (!pimpl->initialized)
  {
    std::cout << "ERROR: Game_Engine not yet initialized\n";
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

  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(pimpl->framerate));

  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(fps_counter);
  LOCK_FUNCTION(update_fps);
  install_int(update_fps, 1000);

  engine_active = true;

  while (engine_active)
  {
    if (ticks > 0)
    {
      while (ticks > 0)
      {
        --ticks;

        if (key[KEY_ESC])
        {
          engine_active = false;
          break;
        }

        if (engine_active)
        {
          pimpl->states.top()->Update(this);
          pimpl->need_redraw = true;
        }
      }

      if (pimpl->need_redraw && engine_active)
      {
        pimpl->need_redraw = false;
        clear_to_color(pimpl->buffer, pimpl->bg_color);

        pimpl->states.top()->Draw(pimpl->buffer);

        if (pimpl->mouse && pimpl->cursor)
        {
          show_mouse(pimpl->buffer);
        }

        blit(pimpl->buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        if (pimpl->mouse && pimpl->cursor)
        {
          show_mouse(0);
        }

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

void Game_Engine::Enable_Cursor(bool enable)
{
  pimpl->cursor = enable;
}

void Game_Engine::Set_BG_Color(int color)
{
  pimpl->bg_color = color;
}
