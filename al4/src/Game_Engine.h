#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)

#define BG_COLOR_DEFAULT  makecol(192, 192, 192)

class State;

class Game_Engine
{
  struct Game_Internal;
  Game_Internal *pimpl;

/******************************************************************************
  Variables
*******************************************************************************/
public:
  // Argument list
  int argc; char **argv;

  // Whether the game is exiting
  bool exiting;

/******************************************************************************
  Methods
*******************************************************************************/
public:
  Game_Engine();
  ~Game_Engine();

  // Main
  bool Init(int _argc, char **_argv, const char *title, int width, int height,
            int rate, bool want_fs, bool want_mouse, bool want_audio,
            int depth = 32);
  void Run(State *first = 0);

  // State manipulation
  void Change_State(State *s);
  void Push_State(State *s);
  void Pop_State();

  // Other
  void Game_Over();
  void Enable_Cursor(bool enable);
  void Set_BG_Color(int color);
};

// Accessible from anywhere
extern volatile int fps;

#endif // GAME_ENGINE_H_INCLUDED
