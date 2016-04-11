#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro.h>

// Main state structure
struct State
{
    // Startup and shutdown
    void (*init)(void*);
    void (*end)(int); // argument will be larger than 0 when the game finishes

    void (*pause)();
    void (*resume)();

    void (*update)();
    void (*draw)(BITMAP*);
};

#endif // STATE_H_INCLUDED
