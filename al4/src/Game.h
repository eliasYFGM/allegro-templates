#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro.h>
#include "State.h"

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)

#define DEFAULT_BG_COLOR  makecol(192, 192, 192)

class Game
{
  struct Game_Internal;
  Game_Internal* intern;

  void Update();
  void Draw();

public:
  Game();
  ~Game();

  bool Init(const char* title, int width, int height, int rate, int depth,
            bool want_fs, bool want_audio);
  void Run();
  void Game_Over(); // Only meant for states; NO need to call after Run()
  void Set_BG_Color(int color);

  // State manipulation
  void Change_State(State* state);
  void Push_State(State* state);
  void Pop_State();
};

// Accessible from anywhere
extern volatile int fps;

#endif // GAME_H_INCLUDED
