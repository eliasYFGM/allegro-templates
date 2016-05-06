#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stack>
#include "State.h"

#define C_BLACK   al_map_rgb(0, 0, 0)
#define C_WHITE   al_map_rgb(255, 255, 255)

class Game
{
  ALLEGRO_DISPLAY* display;
  ALLEGRO_BITMAP* buffer;
  ALLEGRO_TIMER* timer;
  ALLEGRO_EVENT_QUEUE* event_queue;
  ALLEGRO_COLOR bg_color;
  bool need_update;
  bool is_running;
  std::stack<State*> states;

public:
  static int internal_w;
  static int internal_h;

public:
  Game() : display(0), buffer(0), timer(0), event_queue(0)
  {
    need_update = false;
    is_running = false;
  }

  ~Game();

  bool Init(int width, int height, const char* title = "Allegro application",
            bool fullscreen = false, int rate = 40);

  void Handle_Events();
  void Update();
  void Draw();

  bool Running()
  {
    return is_running;
  }

  void Game_Over()
  {
    is_running = false;
  }

  void Set_BG_Color(ALLEGRO_COLOR color)
  {
    bg_color = color;
  }

  void Change_State(State* state);
  void Push_State(State* state);
  void Pop_State();
};

#define SCREEN_W  Game::internal_w
#define SCREEN_H  Game::internal_h

extern ALLEGRO_FONT* font;

#endif // GAME_H_INCLUDED
