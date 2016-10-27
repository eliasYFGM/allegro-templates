#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)

#define BG_COLOR_DEFAULT  makecol(192, 192, 192)

class State;

class Game_Engine
{
  struct Game_Internal;
  static Game_Internal* pimpl;

/******************************************************************************
  Variables
*******************************************************************************/
protected:
  // Argument list
  static int argc;
  static char** argv;

/******************************************************************************
  Methods
*******************************************************************************/
public:
  Game_Engine();

  // These won't do anything outside of main()!
  bool Init(int _argc, char** _argv, const char* title, int width, int height,
            int rate, int depth, bool want_fs, bool want_audio);
  void Run(State* start_state = 0);

protected:
  void Game_Over();
  void Set_BG_Color(int color);

  // State manipulation
  void Change_State(State*);
  void Push_State(State*);
  void Pop_State();
};

// Accessible from anywhere
extern volatile int fps;

#endif // GAME_ENGINE_H_INCLUDED
