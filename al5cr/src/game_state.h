#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED

#include "state.h"

struct State *get_game_state(void);

#define GAME_STATE  get_game_state()

#endif // GAMESTATE_H_INCLUDED
