#include "Game_State.h"
#include "Game_Engine.h"

void Game_State::Init()
{
}

void Game_State::End()
{
}

void Game_State::Pause()
{
}

void Game_State::Resume()
{
}

void Game_State::Events(Game_Engine *engine, ALLEGRO_EVENT& ev)
{
}

void Game_State::Update(Game_Engine *engine)
{
}

void Game_State::Draw()
{
}

State* Game_State::Get_State()
{
  static Game_State state;
  return &state;
}
