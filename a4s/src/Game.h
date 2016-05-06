#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro.h>
#include <stack>
#include "State.h"

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)

class Game
{
  BITMAP* buffer;
  int bg_color;
  bool initialized;
  bool need_redraw;

  std::stack<State*> states;

public:
  Game() : buffer(0), need_redraw(false) {}
  ~Game();

  bool Init(int width, int height, const char* title = "Allegro application",
            bool fullscreen = false, int rate = 40, int depth = 32);
  void Update();
  void Draw();

  bool Running();
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
