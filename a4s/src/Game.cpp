#include <cstdlib>
#include <ctime>
#include "Game.h"

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

Game::~Game()
{
  while (!states.empty())
  {
    delete states.top();
    states.pop();
  }

  if (buffer)
  {
    destroy_bitmap(buffer);
  }
}

bool Game::Init(int width, int height, const char* title, bool fullscreen,
  int rate, int depth)
{
  allegro_init();
  install_keyboard();
  install_mouse();
  install_timer();

  set_color_depth(depth);

  if (set_gfx_mode(
    fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
    width, height, 0, 0))
  {
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    allegro_message("%s\n", allegro_error);
    return false;
  }

  set_window_title(title);

  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  framerate = rate;

  LOCK_FUNCTION(close_button_handler);
  set_close_button_callback(close_button_handler);

  Set_BG_Color(DEFAULT_BG_COLOR);

  srand(time(0));

  return true;
}

void Game::Run()
{
  LOCK_VARIABLE(ticks);
  LOCK_FUNCTION(ticker);
  install_int_ex(ticker, BPS_TO_TIMER(framerate));

  LOCK_VARIABLE(fps);
  LOCK_VARIABLE(fps_counter);
  LOCK_FUNCTION(update_fps);
  install_int(update_fps, 1000);

  is_running = true;

  while(is_running)
  {
    Update();
    Draw();
  }
}

void Game::Update()
{
  if (ticks)
  {
    while (ticks)
    {
      --ticks;
      states.top()->Update(this);
    }

    if (is_running)
    {
      need_redraw = true;
    }
  }
  else
  {
    rest(1);
  }
}

void Game::Draw()
{
  if (need_redraw)
  {
    need_redraw = false;

    clear_to_color(buffer, bg_color);

    states.top()->Draw(this, buffer);

    show_mouse(buffer);

    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    show_mouse(0);

    ++fps_counter;
  }
}

void Game::Game_Over()
{
  is_running = false;
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
