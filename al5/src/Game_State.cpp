#include "Game_State.h"
#include "Game_Engine.h"

Game_State::Game_State(Game_Engine *ge) : engine(ge)
{
}

Game_State::~Game_State()
{
}

void Game_State::Pause()
{
}

void Game_State::Resume()
{
}

void Game_State::Events(ALLEGRO_EVENT& ev)
{
}

void Game_State::Update()
{
}

void Game_State::Draw()
{
}
