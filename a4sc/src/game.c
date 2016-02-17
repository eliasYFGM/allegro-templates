#include <stdio.h>
#include <allegro.h>
#include "game.h"

struct {
    BITMAP* buffer;
    int initialized;
}
game =
{
    NULL,
    0
};

#define MAX_STATES  8

static struct State* states[MAX_STATES];
static int current_s = 0;

static volatile int ticks = 0;

static void ticker()
{
    ++ticks;
}
END_OF_FUNCTION(ticker)

static volatile int frame_counter = 0;

static void update_fps()
{
    fps = frame_counter;
    frame_counter = 0;
}
END_OF_FUNCTION(update_fps)

static volatile int is_running;

#ifndef ALLEGRO_DOS
static void close_button_handler()
{
    is_running = 0;
}
END_OF_FUNCTION(close_button_handler)
#endif // ALLEGRO_DOS

// Main game initialization
int game_init(struct Game_Config config)
{
    int i;

    if (game.initialized)
    {
        puts("WARNING: Game already initialized");
        return 0;
    }

    for (i=0; i<MAX_STATES; ++i)
    {
        states[i] = NULL;
    }

    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();

    if (config.audio)
    {
        if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
        {
            puts("WARNING: Could not initialize audio");
        }
    }

    set_color_depth(config.depth);

#ifdef ALLEGRO_DOS
    if (set_gfx_mode(GFX_AUTODETECT, config.width, config.height, 0, 0))
#else
    if (set_gfx_mode(config.fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
        config.width, config.height, 0, 0))
#endif
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("ERROR: Could not init graphics mode:\n%s", allegro_error);
        return 0;
    }

#ifndef ALLEGRO_DOS
    set_window_title(config.title);
#endif // ALLEGRO_DOS

    game.buffer = create_bitmap(SCREEN_W, SCREEN_H);
    bg_color = makecol(192, 192, 192);

    // Main game timer
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(config.framerate));

    // FPS timer
    LOCK_VARIABLE(fps);
    LOCK_VARIABLE(frame_counter);
    LOCK_FUNCTION(update_fps);
    install_int(update_fps, 1000);

#ifndef ALLEGRO_DOS
    set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

    is_running = 1;
    game.initialized = 1;

    return 1;
}

// Game loop
void game_run()
{
    int i, redraw = 0;

    if (!states[current_s])
    {
        puts("ERROR: change_state was not called");
        return;
    }

    // Game loop
    while (is_running)
    {
        if (ticks)
        {
            while (ticks)
            {
                --ticks;

                if (key[KEY_ESC])
                {
                    is_running = 0;
                    break;
                }

                states[current_s]->update();
                redraw = 1;
            }

            if (is_running && redraw)
            {
                redraw = 0;

                clear_to_color(game.buffer, bg_color);

                states[current_s]->draw(game.buffer);

                show_mouse(game.buffer);
                blit(game.buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                show_mouse(NULL);

                ++frame_counter;
            }
        }
#ifndef ALLEGRO_DOS
        else
        {
            rest(1);
        }
#endif // ALLEGRO_DOS
    }

    for (i=0; i<MAX_STATES; ++i)
    {
        if (states[i] != NULL)
        {
            states[i]->end();
        }
    }

    destroy_bitmap(game.buffer);
}

// Ends the game
void game_over()
{
    is_running = 0;
}

// Changes the state directly to another
void change_state(struct State* state, void* param)
{
    if (states[current_s] != NULL)
    {
        states[current_s]->end();
    }

    states[current_s] = state;
    state->init(param);
}

// Pushes a new state onto the stack (previous one is 'paused')
void push_state(struct State* state, void* param)
{
    if (current_s < (MAX_STATES - 1))
    {
        if (states[current_s] != NULL)
        {
            states[current_s]->pause();
        }

        states[++current_s] = state;
        state->init(param);
    }
    else
    {
        puts("WARNING: Can't add new state (current_s = MAX_STATES)");
    }
}

// Removes the last state added from the stack
void pop_state()
{
    if (current_s > 0)
    {
        states[current_s]->end();
        states[current_s] = NULL;
        states[--current_s]->resume();
    }
    else
    {
        puts("WARNING: Can't remove any more states (current_s = 0)");
    }
}
