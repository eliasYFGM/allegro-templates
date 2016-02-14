#include <iostream>
#include <allegro.h>
#include "gameengine.h"

bool Game_Engine::is_running;

static volatile int ticks = 0;

static void ticker()
{
    ++ticks;
}
END_OF_FUNCTION(ticker)

volatile int fps = 0, frame_counter = 0;

static void update_fps()
{
    fps = frame_counter;
    frame_counter = 0;
}
END_OF_FUNCTION(update_fps)

#ifndef ALLEGRO_DOS
static void close_button_handler()
{
    Game_Engine::is_running = false;
}
END_OF_FUNCTION(close_button_handler)
#endif // ALLEGRO_DOS

bool Game_Engine::init(const char* title, int w, int h, int rate, int depth, bool fullscreen, bool audio)
{
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();

    if (audio)
    {
        if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0))
        {
            std::cout << "Could not initialize audio" << std::endl;
            return false;
        }
    }

    set_color_depth(depth);

#ifdef ALLEGRO_DOS
    if (set_gfx_mode(GFX_AUTODETECT, w, h, 0, 0))
#else
    if (set_gfx_mode(fullscreen ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED, w, h, 0, 0))
#endif
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("Could not initialize GFX:\n%s", allegro_error);
        return false;
    }

#ifndef ALLEGRO_DOS
    set_window_title(title);
#endif // ALLEGRO_DOS

    buffer = create_bitmap(w, h);
    bg_color = makecol(192, 192, 192);

    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(rate));

    LOCK_VARIABLE(fps);
    LOCK_VARIABLE(frame_counter);
    LOCK_FUNCTION(update_fps);
    install_int(update_fps, 1000);

#ifndef ALLEGRO_DOS
    set_close_button_callback(close_button_handler);
#endif // ALLEGRO_DOS

    is_running = true;

    return true;
}

void Game_Engine::run()
{
    bool redraw = false;

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
                    is_running = false;
                    break;
                }

                state_list.top()->update(this);

                redraw = true;
            }

            if (is_running && redraw)
            {
                redraw = false;

                clear_to_color(buffer, bg_color);

                state_list.top()->draw(this, buffer);

                show_mouse(buffer);
                blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

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

    while (!state_list.empty())
    {
        delete state_list.top();
        state_list.pop();
    }

    destroy_bitmap(buffer);
}

void Game_Engine::change_state(State* state)
{
    if (!state_list.empty())
    {
        delete state_list.top();
        state_list.pop();
    }

    state_list.push(state);
}

void Game_Engine::push_state(State* state)
{
    if (!state_list.empty())
    {
        state_list.top()->pause(this);
    }

    state_list.push(state);
}

void Game_Engine::pop_state()
{
    if (!state_list.empty())
    {
        delete state_list.top();
        state_list.pop();
    }

    if (!state_list.empty())
    {
        state_list.top()->resume(this);
    }
    else
    {
        is_running = false;
    }
}
