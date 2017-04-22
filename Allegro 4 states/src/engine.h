#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#define C_BLACK		makecol(0, 0, 0)
#define C_WHITE		makecol(255, 255, 255)
#define C_TRANS		makecol(255, 0, 255)

// Max states to allocate
#define MAX_STATES	8

struct Engine_Conf
  {
    // Argument list
    int argc;
    char **argv;

    // Other needed parameters
    char *title;
    int width;
    int height;
    int framerate;
    int depth;
    int fullscreen;
  };

// Defined in state.h
struct State;

/*******************************************************************************
  Main
*******************************************************************************/

// Initializes the engine with a configuration structure.
int engine_init(struct Engine_Conf *conf);

// Run the engine with a starting state, until [engine_active = FALSE].
void engine_run(struct State *s);

/*******************************************************************************
  State manipulation
*******************************************************************************/

// change_state() - Changes the state directly to another.
// "param" is a pointer that can be passed to the new state. Use NULL to not
// pass anything.
void change_state(struct State *s, void *param);

// push_state() - Add a new state to the stack (previous one is 'paused').
// "param" is a pointer that can be passed to the new state. Use NULL to not
// pass anything.
void push_state(struct State *s, void *param);

// pop_state() - removes the last state added with push_state().
void pop_state(void);

/*******************************************************************************
  Misc functions
*******************************************************************************/
void game_over(void);
void enable_cursor(int enable);
void set_bg_color(int c);

/*******************************************************************************
  Globals
*******************************************************************************/

// FPS is updated each second.
extern volatile int fps;

// Pointer to the original settings (in main.c).
extern const struct Engine_Conf *MAINCONF;

// Bounding box collision (taken from Alex4)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
      ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // ENGINE_H_INCLUDED
