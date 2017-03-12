#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#define C_BLACK   makecol(0, 0, 0)
#define C_WHITE   makecol(255, 255, 255)
#define C_TRANS   makecol(255, 0, 255)

#define BG_COLOR_DEFAULT  makecol(192, 192, 192)

// Max states to allocate
#define MAX_STATES  8

// Defined in state.h
struct State;

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
  int mouse;
  int audio;
};

// Main
int engine_init(struct Engine_Conf *conf);
void engine_run(struct State *first);

// States
void change_state(struct State *s, void *param);
void push_state(struct State *s, void *param);
void pop_state(void);

// Other
void enable_cursor(int enable);
void set_bg_color(int c);

// Whether the engine is active (started or running)
// set to FALSE to stop
extern volatile int engine_active;

// FPS is updated each second
extern volatile int fps;

// Pointer to the original settings (in main.c)
extern const struct Engine_Conf *mainconf;

// Bounding box collision (taken from Alex4)
#define check_bb_collision(x1,y1,w1,h1,x2,y2,w2,h2) \
  (!( ((x1)>=(x2)+(w2)) || ((x2)>=(x1)+(w1)) || \
      ((y1)>=(y2)+(h2)) || ((y2)>=(y1)+(h1)) ))

#endif // ENGINE_H_INCLUDED
