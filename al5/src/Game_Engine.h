#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <allegro5/allegro_font.h>

#define C_BLACK   al_map_rgb(0, 0, 0)
#define C_WHITE   al_map_rgb(255, 255, 255)

#define BG_COLOR_DEFAULT  al_map_rgb(192, 192, 192)

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

  // Array holding key presses
  static bool key[ALLEGRO_KEY_MAX];

  // Default fixed-width font
  static ALLEGRO_FONT* font;

/******************************************************************************
  Methods
*******************************************************************************/
public:
  Game_Engine();

  // These won't do anything outside of main()!
  bool Init(int _argc, char** _argv, const char* title, int width, int height,
            int rate, bool want_fs, bool want_audio, bool want_bb);
  void Run(State* start_state = 0);

protected:
  void Game_Over();
  void Set_BG_Color(ALLEGRO_COLOR);
  void Get_Internal_Res(int& w, int& h);

  // State manipulation
  void Change_State(State*);
  void Push_State(State*);
  void Pop_State();
};

#endif // GAME_ENGINE_H_INCLUDED
