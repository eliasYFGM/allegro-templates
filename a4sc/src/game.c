#include <stdio.h>
#include <allegro.h>
#include "game.h"

// Max number of states
#define MAX_STATES  8

// Stack of states
static struct State* states[MAX_STATES];
static int current_state = 0;

// Back-buffer where the main drawing takes place
static BITMAP* backbuffer;

// Just to check whether the game was already initialized
static int initialized = FALSE;

// Each tick = new frame
static volatile int ticks = 0;

static void ticker()
{
    ++ticks;
}
END_OF_FUNCTION(ticker)

// The game will keep running until is set to '0' (done through game_over() )
static volatile int is_running;

#ifndef ALLEGRO_DOS
static void close_button_handler()
{
    is_running = FALSE;
}
END_OF_FUNCTION(close_button_handler)
#endif // ALLEGRO_DOS

// Main game initialization
int game_init(struct Game_Config* config, const char* title)
{
    int i;

    if (initialized)
    {
        puts("WARNING: Game already initialized");
        return 0;
    }

    for (i=0; i<MAX_STATES; ++i)
    {
        states[i] = NULL;
    }

    // Initialize Allegro
    allegro_init();

    // Install modules
    install_keyboard();
    install_mouse();
    install_timer();

    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0))
    {
        puts("WARNING: Could not initialize audio");
    }

    set_color_depth(config->depth);

#ifdef ALLEGRO_DOS
    if (set_gfx_mode(GFX_AUTODETECT, config->width, config->height, 0, 0))
#else
    if (set_gfx_mode(config->fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED,
        config->width, config->height, 0, 0))
#endif
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("ERROR: Could not init graphics mode:\n%s", allegro_error);
        return 0;
    }

#ifndef ALLEGRO_DOS
    // Set the window/display title
    set_window_title(title);
#endif // ALLEGRO_DOS

    // Internal game screen
    backbuffer = create_bitmap(SCREEN_W, SCREEN_H);

    // Background color
    bg_color = makecol(192, 192, 192);

    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);

    // Create main timer
    install_int_ex(ticker, BPS_TO_TIMER(config->framerate));

#ifndef ALLEGRO_DOS
    set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

    is_running = TRUE;
    initialized = TRUE;

    return 1;
}

// Game loop
void game_run()
{
    int i, redraw = FALSE;

    if (!states[current_state])
    {
        puts("ERROR: change_state was not called");
        return;
    }

    // This is the main game loop
    while (is_running)
    {
        if (ticks)
        {
            while (ticks)
            {
                --ticks;

                if (key[KEY_ESC])
                {
                    is_running = FALSE;
                    break;
                }

                states[current_state]->update();
                redraw = TRUE;
            }

            if (is_running && redraw)
            {
                redraw = FALSE;

                clear_to_color(backbuffer, bg_color);

                states[current_state]->draw(backbuffer);

                show_mouse(backbuffer);
                blit(backbuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                show_mouse(NULL);
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

    destroy_bitmap(backbuffer);
}

// Ends the game
void game_over()
{
    is_running = FALSE;
}

// Changes the state directly to another
void change_state(struct State* state, void* param)
{
    if (states[current_state] != NULL)
    {
        states[current_state]->end();
    }

    states[current_state] = state;
    state->init(param);
}

// Pushes a new state onto the stack (previous one is 'paused')
void push_state(struct State* state, void* param)
{
    if (current_state < MAX_STATES)
    {
        if (states[current_state] != NULL)
        {
            states[current_state]->pause();
        }

        states[++current_state] = state;
        state->init(param);
    }
    else
    {
        puts("WARNING: Can't add new state (current_state = MAX_STATES)");
    }
}

// Removes the last state added from the stack
void pop_state()
{
    if (current_state > 0)
    {
        states[current_state]->end();
        states[current_state] = NULL;
        states[--current_state]->resume();
    }
    else
    {
        puts("WARNING: Can't remove any more states (current_state = 0)");
    }
}
