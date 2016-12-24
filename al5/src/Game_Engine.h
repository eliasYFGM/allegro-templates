#ifndef GAME_ENGINE_H_INCLUDED
#define GAME_ENGINE_H_INCLUDED

#include <memory>
#include <string>
#include <vector>
#include <allegro5/allegro_font.h>

#define C_BLACK   al_map_rgb(0, 0, 0)
#define C_WHITE   al_map_rgb(255, 255, 255)

#define BG_COLOR_DEFAULT  al_map_rgb(192, 192, 192)

class State;

class Game_Engine
{
  struct Impl;
  std::unique_ptr<Impl> pimpl;

public:
  // Argument list
  std::vector<std::string> args;

  // Original width and height
  int width, height;

public:
  Game_Engine();
  ~Game_Engine();

  // Main
  bool Init(int argc, char **argv, const char *title, int w, int h, int rate,
            bool full, bool audio, bool backbuff);
  void Run(State *first = 0);

  // States
  void Change_State(State *s);
  void Push_State(State *s);
  void Pop_State();

  // Other
  void Set_BG_Color(ALLEGRO_COLOR c);
};

// Whether the engine is active (started or running)
// set to 'false' to stop
extern bool engine_active;

// Array holding key presses
extern bool key[ALLEGRO_KEY_MAX];

// Default fixed-width font
extern ALLEGRO_FONT *font;

#endif // GAME_ENGINE_H_INCLUDED
