#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#define C_BLACK   al_map_rgb(0, 0, 0)
#define C_WHITE   al_map_rgb(255, 255, 255)

#define BG_COLOR_DEFAULT  al_map_rgb(192, 192, 192)

class State_Object;

class Game
{
  struct Game_Internal;
  static Game_Internal* pimpl;

public:
  // These won't do anything outside of the main() function
  bool Init(const char* title, int width, int height, int rate,
            bool want_fs, bool want_audio, bool want_bb);
  void Run(State_Object* start_state = 0);

protected:
  void Game_Over();
  void Set_BG_Color(ALLEGRO_COLOR);
  void Get_Internal_Res(int& w, int& h);

  void Change_State(State_Object*);
  void Push_State(State_Object*);
  void Pop_State();

protected:
  // Default fixed-width font
  static ALLEGRO_FONT* font;

  // Array holding key presses, only for the state's Update() function
  static bool keys[ALLEGRO_KEY_MAX];
};

#endif // GAME_H_INCLUDED
