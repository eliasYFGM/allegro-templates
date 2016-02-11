#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "gameengine.h"

static void aspect_ratio_transform(ALLEGRO_DISPLAY* display)
{
    int window_w = al_get_display_width(display);
    int window_h = al_get_display_height(display);

    float sw = (window_w / (float)(SCREEN_W));
    float sh = (window_h / (float)(SCREEN_H));
    float scale = std::min(sw, sh);

    float scale_w = (SCREEN_W * scale);
    float scale_h = (SCREEN_H * scale);
    int scale_x_pos = (window_w - scale_w) / 2;
    int scale_y_pos = (window_h - scale_h) / 2;

    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_build_transform(&trans, scale_x_pos, scale_y_pos, scale, scale, 0);
    al_use_transform(&trans);
}

int SCREEN_W = 0;
int SCREEN_H = 0;
ALLEGRO_FONT* font = 0;

bool Game_Engine::init(int w, int h, bool fullscreen, int rate, bool audio, const char* title)
{
    // Init Allegro and stuff
    al_init();

    if (!al_install_keyboard())
    {
        std::cout << "Could not initialize the keyboard...\n";
        return false;
    }

    if (!al_install_mouse())
    {
        std::cout << "Could not initialize the mouse...\n";
        return false;
    }

    if (audio)
    {
        if (!al_install_audio())
        {
            std::cout << "Could not initialize audio...\n";
            return false;
        }

        if (!al_init_acodec_addon())
        {
            std::cout << "Could not initialize acodec addon...\n";
            return false;
        }
    }

    // Addons
    if (!al_init_image_addon())
    {
        std::cout << "Could not initialize image addon...\n";
        return false;
    }

    al_init_font_addon();

    if (fullscreen)
    {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    }

    SCREEN_W = w;
    SCREEN_H = h;

    // Create our display
    display = al_create_display(SCREEN_W, SCREEN_H);

    if (!display)
    {
        std::cout << "Could not create a display window...\n";
        return false;
    }

    // Set the window/display title
    al_set_window_title(display, title);

    // Use linear filtering for scaling game screen
    al_add_new_bitmap_flag(ALLEGRO_MAG_LINEAR);

    // Backbuffer
    buffer = al_create_bitmap(SCREEN_W, SCREEN_H);

    // Update the aspect ratio
    aspect_ratio_transform(display);

    // Use built-in Allegro font
    font = al_create_builtin_font();

    // Background color
    bg_color = al_map_rgb(192, 192, 192);

    // Create our timer (FPS handler)
    timer = al_create_timer(1.0 / rate);

    // Create our event queue
    event_queue = al_create_event_queue();

    // We need to tell Allegro which events we'll use
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    is_running = true;

    return true;
}

void Game_Engine::run()
{
    bool redraw = false;

    // This is the main game loop
    while (is_running)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // TODO: Process events state
        state_list.top()->process_events(this, event);

        // If the close button was pressed...
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            is_running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // Escape key will end the game
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            {
                is_running = false;
            }

            // The F4 key will switch between screen modes (mantaining aspect ratio)
            // Inspired by Game Maker's functionality, although with some fixes (no black bars)
            if (event.keyboard.keycode == ALLEGRO_KEY_F4)
            {
                if (al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW)
                {
                    al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);
                }
                else
                {
                    al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
                }

                aspect_ratio_transform(display);
            }
        }
        else if (event.type == ALLEGRO_EVENT_TIMER)
        {
            state_list.top()->update(this);
            redraw = true;
        }

        if (redraw && al_event_queue_is_empty(event_queue))
        {
            redraw = false;

            al_set_target_bitmap(buffer);

            al_clear_to_color(bg_color);

            state_list.top()->draw(this);

            al_set_target_backbuffer(display);

            al_draw_bitmap(buffer, 0, 0, 0);

            al_flip_display();
        }
    }
}

void Game_Engine::end()
{
    while (!state_list.empty())
    {
        delete state_list.top();
        state_list.pop();
    }

    al_destroy_display(display);
    al_destroy_bitmap(buffer);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
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
