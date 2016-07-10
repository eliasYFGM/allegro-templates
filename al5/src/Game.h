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

  bool Init(int width, int height, const char* title = "Allegro application",
            bool fullscreen = false, int rate = 40);
  void Run();
  void Game_Over();
  void Set_BG_Color(ALLEGRO_COLOR color);
  void Get_Original_Res(int& w, int& h);

  void Change_State(State* state);
  void Push_State(State* state);
  void Pop_State();
};

extern ALLEGRO_FONT* font;

#endif // GAME_H_INCLUDED
