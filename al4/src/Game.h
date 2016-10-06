#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)

#define DEFAULT_BG_COLOR  makecol(192, 192, 192)

class State_Object;

class Game
{
  struct Game_Internal;
  static Game_Internal* intern;

public:
  // These won't do anything outside of main()
  bool Init(const char* title, int width, int height, int rate, int depth,
            bool want_fs, bool want_audio);
  void Run(State_Object* start_state = 0);

protected:
  void Game_Over();
  void Set_BG_Color(int color);

  // State manipulation
  void Change_State(State_Object*);
  void Push_State(State_Object*);
  void Pop_State();
};

// Accessible from anywhere
extern volatile int fps;

#endif // GAME_H_INCLUDED
