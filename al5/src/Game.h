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
  struct Game_Internal;
  Game_Internal* intern;

  void Handle_Events();
  void Update();
  void Draw();

public:
  Game();
  ~Game();

  bool Init(const char* title, int width, int height, int rate, bool want_fs,
            bool want_audio, bool want_bb);
  void Run();
  void Game_Over();
  void Set_BG_Color(ALLEGRO_COLOR color);
  void Get_Original_Res(int& w, int& h);

  void Change_State(State* state);
  void Push_State(State* state);
  void Pop_State();
};

// Default fixed-width font
extern ALLEGRO_FONT* font;

// Array holding key presses, only for the state's Update() function
extern bool keys[ALLEGRO_KEY_MAX];

#endif // GAME_H_INCLUDED
