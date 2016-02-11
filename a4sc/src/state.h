#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

#include <allegro.h>

// Main State structure
struct State
{
    void (*init)(void*);
    void (*end)();

    void (*pause)();
    void (*resume)();

    void (*update)();
    void (*draw)(BITMAP*);
};

#endif // STATE_H_INCLUDED
