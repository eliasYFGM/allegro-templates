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
  Game_Internal *pimpl;

public:
  // Argument list
  int argc; char **argv;

  // Original width and height
  int width, height;

  // Whether the game is exiting
  bool exiting;

public:
  Game_Engine();
  ~Game_Engine();

  // Main
  bool Init(int _argc, char **_argv, const char *title, int _width, int _height,
            int rate, bool want_fs, bool want_audio, bool want_bb);
  void Run(State *first = 0);

  // State manipulation
  void Change_State(State *s);
  void Push_State(State *s);
  void Pop_State();

  // Other
  void Game_Over();
  void Set_BG_Color(ALLEGRO_COLOR c);
};

// Array holding key presses
extern bool key[ALLEGRO_KEY_MAX];

// Default fixed-width font
extern ALLEGRO_FONT *font;

#endif // GAME_ENGINE_H_INCLUDED
