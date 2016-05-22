#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro.h>
#include <stack>
#include "State.h"

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)
#define DEFAULT_BG_COLOR  makecol(192, 192, 192)

class Game
{
  BITMAP* buffer;
  bool need_redraw;
  int bg_color;
  int framerate;
  std::stack<State*> states;

  void Update();
  void Draw();

public:
  Game() : buffer(0), need_redraw(false) {}
  ~Game();

  bool Init(int width, int height, const char* title = "Allegro application",
            bool fullscreen = false, int rate = 40, int depth = 32);
  void Run();

  void Game_Over();

  void Set_BG_Color(int color)
  {
    bg_color = color;
  }

  void Change_State(State* state);
  void Push_State(State* state);
  void Pop_State();
};

extern volatile int fps;

#endif // GAME_H_INCLUDED
