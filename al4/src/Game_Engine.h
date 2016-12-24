#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <memory>
#include <string>
#include <vector>

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)

#define BG_COLOR_DEFAULT  makecol(192, 192, 192)

class State;

class Game_Engine
{
  struct Impl;
  std::unique_ptr<Impl> pimpl;

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------
public:
  // Argument list
  std::vector<std::string> args;

//------------------------------------------------------------------------------
// Methods
//------------------------------------------------------------------------------
public:
  Game_Engine();
  ~Game_Engine();

  // Main
  bool Init(int argc, char **argv, const char *title, int width, int height,
            int rate, int depth, bool full, bool mouse, bool audio);
  void Run(State *first = 0);

  // States
  void Change_State(State *s);
  void Push_State(State *s);
  void Pop_State();

  // Other
  void Enable_Cursor(bool enable);
  void Set_BG_Color(int color);
};

// Whether the engine is active (started or running)
// set to 'false' to stop
extern volatile bool engine_active;

// FPS accessible from anywhere
extern volatile int fps;

#endif // GAME_ENGINE_H_INCLUDED
